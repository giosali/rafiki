#include "indexer.h"

#include <string>
#include <utility>

#include "INIReader.h"
#include "fetcher.h"
#include "file.h"
#include "models/applicationmodel.h"
#include "models/calculatormodel.h"
#include "models/filesystementrymodel.h"
#include "models/trashmodel.h"
#include "models/urlmodel.h"
#include "models/websearchmodel.h"
#include "paths.h"
#include "settings.h"

Indexer& Indexer::GetInstance() {
  // Thread-safe magic static.
  static auto instance = Indexer{};
  return instance;
}

void Indexer::DeleteModel(uint64_t id) {
  if (!models_map_.contains(id)) {
    return;
  }

  auto model = models_map_[id].get();
  auto tokens = model->Tokenize();
  for (const auto& token : tokens) {
    auto it = models_trie_.find(token);
    if (it == models_trie_.end()) {
      continue;
    }

    // Gets a reference to the corresponding id set in the trie.
    // If there is only one id in the id set, the entire set is removed.
    // Otherwise, only the id matching the model id is removed.
    auto& ids = it.value();
    if (ids.size() == 1) {
      models_trie_.erase(token);
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
  auto key = input.toLower().toStdString();

  if (auto i = key.find_first_not_of(' '); i != 0) {
    // Means that the input consists of only spaces, and so an empty set will be
    // returned to force default results to be shown.
    if (i == std::string::npos) {
      return {};
    }

    // Trims left side of input.
    key = key.substr(i);
  }

  // Attempts to extract everything before the first space character and use
  // that as a key for the trie containing the models.
  if (auto i = key.find(' '); i != std::string::npos) {
    key = key.substr(0, i + 1);
  }

  // Includes ids from models that don't contain a command by default.
  auto ids = commandless_ids_;

  auto range = models_trie_.equal_prefix_range(key);
  for (auto it = range.first; it != range.second; ++it) {
    auto value = it.value();
    ids.insert(value.begin(), value.end());
  }

  return ids;
}

FeatureModel* Indexer::GetModel(uint64_t id) const {
  return models_map_.at(id).get();
}

void Indexer::IndexModel(std::unique_ptr<FeatureModel> model) {
  auto id = model->GetId();

  if (model->GetCommand().isNull()) {
    commandless_ids_.insert(id);
  } else {
    // Inserts Result ID into tokens of FeatureModel. This must be done prior
    // to moving the unique_ptr to the model.
    auto tokens = model->Tokenize();
    for (const auto& token : tokens) {
      auto pair = models_trie_.insert(token, std::unordered_set<uint64_t>{});
      pair.first->insert(id);
    }
  }

  // Maps ID to FeatureModel instance.
  models_map_.insert({id, std::move(model)});
}

void Indexer::Initialize() {
  auto& settings = Settings::GetInstance();
  auto document = File::Read(Paths::Json::kUserSettings);
  settings.Update(document);

  IndexApplications();
  IndexGenericModels();
  IndexWebSearches();

  for (auto id : settings.GetDisabledFeatureModelids()) {
    models_map_.at(id)->SetIsEnabled(false);
  }

  for (auto [id, count] : settings.GetUseCounts()) {
    models_map_.at(id)->SetUseCount(count);
  }
}

void Indexer::ToggleModel(uint64_t id) const {
  auto model = models_map_.at(id).get();
  model->SetIsEnabled(!model->GetIsEnabled());
}

void Indexer::UpdateTrie(uint64_t id,
                         const std::unordered_set<std::string>& old_tokens,
                         const std::unordered_set<std::string>& new_tokens) {
  // Removes old tokens.
  for (const auto& token : old_tokens) {
    auto it = models_trie_.find(token);
    if (it == models_trie_.end()) {
      continue;
    }

    // Gets a reference to the corresponding id set in the trie.
    // If there is only one id in the id set, the entire set is removed.
    // Otherwise, only the id matching the model id is removed.
    auto& ids = it.value();
    if (ids.size() == 1) {
      models_trie_.erase(token);
      continue;
    } else {
      ids.erase(id);
    }
  }

  // Adds new tokens.
  for (const auto& token : new_tokens) {
    auto pair = models_trie_.insert(token, std::unordered_set<uint64_t>{});
    pair.first->insert(id);
  }
}

void Indexer::IndexApplications() {
  auto fetcher = Fetcher{};
  for (const auto& path : fetcher.FetchDesktopEntryPaths()) {
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
    IndexModel(std::make_unique<ApplicationModel>(path, reader));
  }
}

void Indexer::IndexGenericModels() {
  IndexModel(std::make_unique<CalculatorModel>());
  IndexModel(std::make_unique<FileSystemEntryModel>());
  IndexModel(std::make_unique<TrashModel>());
  IndexModel(std::make_unique<UrlModel>());
}

void Indexer::IndexWebSearches() {
  auto fetcher = Fetcher{};
  for (const auto& object : fetcher.FetchWebSearchObjects()) {
    IndexModel(std::make_unique<WebSearchModel>(object));
  }
}
