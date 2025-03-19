#include "indexer.h"

#include <array>
#include <string>
#include <utility>

#include "../core/utilities.h"
#include "INIReader.h"
#include "fetcher.h"
#include "models/applicationmodel.h"
#include "models/calculatormodel.h"
#include "models/filesystementrymodel.h"
#include "models/refreshmodel.h"
#include "models/settingsmodel.h"
#include "models/trashmodel.h"
#include "models/urlmodel.h"
#include "models/websearchmodel.h"
#include "settings.h"

Indexer& Indexer::GetInstance() {
  // Thread-safe magic static.
  static auto instance = Indexer{};
  return instance;
}

void Indexer::Clear() {
  commandless_ids_.clear();
  input_models_trie_.clear();
  inputless_models_trie_.clear();
  models_map_.clear();
}

void Indexer::DeleteModel(uint64_t id) {
  auto model = GetModel(id);
  if (model == nullptr) {
    return;
  }

  auto& trie =
    model->ReceivesInput() ? input_models_trie_ : inputless_models_trie_;
  for (const auto& token : model->Tokenize()) {
    auto it = trie.find(token);
    if (it == trie.end()) {
      continue;
    }

    // Gets a reference to the corresponding id set in the trie.
    // If there is only one id in the id set, the entire set is removed.
    // Otherwise, only the id matching the model id is removed.
    if (auto& ids = it.value(); ids.size() == 1) {
      trie.erase(token);
      continue;
    } else {
      ids.erase(id);
    }
  }

  // This must be performed last because the unique_ptr to the model will be
  // nullptr otherwise.
  models_map_.erase(id);
}

std::unordered_set<uint64_t> Indexer::GetIds(const QString& input) const {
  auto key = Utilities::RemoveAccents(input).toLower().toStdString();

  if (auto i = key.find_first_not_of(' '); i != 0) {
    // Means that the input consists of only spaces, and so an empty set will be
    // returned to force default results to be shown.
    if (i == std::string::npos) {
      return {};
    }

    // Trims left side of input.
    key = key.substr(i);
  }

  // Includes ids from models that don't contain a command by default.
  auto ids = commandless_ids_;

  // The trie with inputless models will have to reqire its keys (or prefixes)
  // to match the entire input.
  auto range = inputless_models_trie_.equal_prefix_range(key);
  for (auto it = range.first; it != range.second; ++it) {
    auto value = it.value();
    ids.insert(value.begin(), value.end());
  }

  // Attempts to extract everything before the first space character and use
  // that as a key for the trie containing the models.
  if (auto i = key.find(' '); i != std::string::npos) {
    key = key.substr(0, i + 1);
  }

  range = input_models_trie_.equal_prefix_range(key);
  for (auto it = range.first; it != range.second; ++it) {
    auto value = it.value();
    ids.insert(value.begin(), value.end());
  }

  return ids;
}

FeatureModel* Indexer::GetModel(uint64_t id) const {
  auto it = models_map_.find(id);
  return it != models_map_.end() ? it->second.get() : nullptr;
}

void Indexer::IndexModel(std::unique_ptr<FeatureModel> model) {
  auto id = model->GetId();

  if (model->GetCommand().isEmpty()) {
    commandless_ids_.insert(id);
  } else {
    auto receives_input = model->ReceivesInput();

    // Inserts Result ID alongside tokens of FeatureModel. This must be done
    // prior to moving the unique_ptr to the model.
    for (const auto& token : model->Tokenize()) {
      auto pair = (receives_input ? input_models_trie_ : inputless_models_trie_)
                    .insert(token, std::unordered_set<uint64_t>{});
      pair.first->insert(id);
    }
  }

  // Maps ID to FeatureModel instance.
  models_map_.insert({id, std::move(model)});
}

void Indexer::Initialize() {
  auto& settings = Settings::GetInstance();

  IndexApplications(QLocale{settings.GetLanguage(), settings.GetTerritory()});
  IndexGenericModels();
  IndexWebSearches();

  for (auto id : settings.GetDisabledFeatureModelids()) {
    if (auto model = GetModel(id); model != nullptr) {
      model->SetIsEnabled(false);
    }
    // Removes unnecessary, leftover disabled IDs.
    else {
      settings.RemoveDisabledFeatureModelId(id);
      settings.Save();
    }
  }

  for (auto [id, timestamps] : settings.GetUsageTimes()) {
    if (auto model = GetModel(id); model != nullptr) {
      model->SetTimestamps(timestamps);
    }
    // Removes unnecessary, leftover usage times.
    else {
      settings.RemoveUsageTimes(id);
      settings.Save();
    }
  }
}

void Indexer::Reinitialize() {
  Clear();
  Initialize();
}

void Indexer::UpdateTrie(uint64_t id,
                         const std::unordered_set<std::string>& old_tokens,
                         const std::unordered_set<std::string>& new_tokens) {
  // When we're removing tokens here, we only need to remove them from one
  // trie: either the one that contains input models or the one that contains
  // inputless models. However, when this function is called, there's a
  // possibility that member function `ReceivesInput` on the model will return a
  // different result than before. In order to account for this, we need to
  // remove the tokens that are associated with the given id.
  auto tries =
    std::array<tsl::htrie_map<char, std::unordered_set<uint64_t>>*, 2>{
      &input_models_trie_, &inputless_models_trie_};

  // Removes old tokens.
  for (const auto& token : old_tokens) {
    for (const auto trie : tries) {
      auto it = trie->find(token);
      if (it == trie->end()) {
        continue;
      }

      // This is necessary because we're checking both tries as explained
      // previously.
      auto& ids = it.value();
      if (!ids.contains(id)) {
        continue;
      }

      // Gets a reference to the corresponding id set in the trie.
      // If there is only one id in the id set, the entire set is removed.
      // Otherwise, only the id matching the model id is removed.
      if (ids.size() == 1) {
        trie->erase(token);
        continue;
      } else {
        ids.erase(id);
      }
    }
  }

  auto model = GetModel(id);
  if (model == nullptr) {
    return;
  }

  // Adds new tokens.
  auto receives_input = model->ReceivesInput();
  for (const auto& token : new_tokens) {
    auto pair = (receives_input ? input_models_trie_ : inputless_models_trie_)
                  .insert(token, std::unordered_set<uint64_t>{});
    pair.first->insert(id);
  }
}

void Indexer::IndexApplications(const QLocale& locale) {
  for (const auto& path : Fetcher{}.FetchDesktopEntryPaths()) {
    auto reader = INIReader{path};
    if (reader.ParseError() < 0) {
      continue;
    }

    // Ignores desktop entries that have NoDisplayset to true. NoDisplay means
    // "this application exists, but don't display it in the menus."
    // Most desktop entry files that are meant to be displayed won't have a
    // NoDisplay key, so that should default to false.
    auto no_display = reader.GetBoolean("Desktop Entry", "NoDisplay", false);
    if (no_display) {
      continue;
    }

    // Maps ID to Application instance.
    IndexModel(std::make_unique<ApplicationModel>(path, reader, locale));
  }
}

void Indexer::IndexGenericModels() {
  IndexModel(std::make_unique<CalculatorModel>());
  IndexModel(std::make_unique<FileSystemEntryModel>());
  IndexModel(std::make_unique<RefreshModel>());
  IndexModel(std::make_unique<SettingsModel>());
  IndexModel(std::make_unique<TrashModel>());
  IndexModel(std::make_unique<UrlModel>());
}

void Indexer::IndexWebSearches() {
  for (const auto& object : Fetcher{}.FetchWebSearchObjects()) {
    IndexModel(std::make_unique<WebSearchModel>(object));
  }
}
