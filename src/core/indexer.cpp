#include "indexer.h"

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

std::unordered_set<uint64_t> Indexer::GetIds(const std::string& input) const {
  // Includes ids from models that don't contain a command by default.
  auto ids = std::unordered_set<uint64_t>{commandless_ids_.begin(),
                                          commandless_ids_.end()};

  auto range = models_trie_.equal_prefix_range(input);
  for (auto it = range.first; it != range.second; ++it) {
    auto value = it.value();
    ids.insert(value.begin(), value.end());
  }

  return ids;
}

FeatureModel* Indexer::GetModel(uint64_t id) const {
  return models_map_.at(id).get();
}

void Indexer::Initialize() {
  auto& settings = Settings::GetInstance();
  auto document = File::Read(Paths::Json::kUserSettings);
  settings.Update(document);

  IndexApplications();
  IndexGenericModels();
  IndexWebSearches();
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

void Indexer::IndexModel(std::unique_ptr<FeatureModel> model) {
  // Maps ID to Result instance.
  auto id = model->GetId();

  if (model->GetCommand().isNull()) {
    commandless_ids_.insert(id);
  } else {
    // Inserts Result ID into tokens of Result. This must be done prior to
    // moving the unique_ptr to the model.
    auto tokens = model->Tokenize();
    for (const auto& token : tokens) {
      auto pair = models_trie_.insert(token, std::unordered_set<uint64_t>{});
      pair.first->insert(id);
    }
  }

  models_map_.insert({id, std::move(model)});
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
