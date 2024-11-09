#include "indexer.h"

#include "../models/application.h"
#include "../models/websearch.h"
#include "INIReader.h"
#include "fetcher.h"
#include "file.h"
#include "paths.h"
#include "settings.h"

Indexer& Indexer::GetInstance() {
  // Thread-safe magic static.
  static auto instance = Indexer{};
  return instance;
}

std::unordered_map<uint64_t, std::shared_ptr<Result>> Indexer::GetResultsMap()
  const {
  return results_map_;
}

tsl::htrie_map<char, std::unordered_set<uint64_t>> Indexer::GetResultsTrie()
  const {
  return results_trie_;
}

void Indexer::Initialize() {
  auto& settings = Settings::GetInstance();
  auto document = File::Read(Paths::Json::kUserSettings);
  settings.Update(document);

  IndexApplications();
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
    auto application = std::make_shared<Application>(path, reader);
    auto id = application->GetId();
    results_map_.insert({id, application});

    // Inserts Application ID into trie.
    auto tokens = application->Tokenize();
    for (const auto& token : tokens) {
      auto pair = results_trie_.insert(token, std::unordered_set<uint64_t>{});
      pair.first->insert(id);
    }
  }
}

void Indexer::IndexWebSearches() {
  auto fetcher = Fetcher{};
  for (const auto& object : fetcher.FetchWebSearchObjects()) {
    // Maps ID to WebSearch instance.
    auto web_search = std::make_shared<WebSearch>(object);
    auto id = web_search->GetId();
    results_map_.insert({id, web_search});

    // Inserts WebSearch ID into trie.
    auto tokens = web_search->Tokenize();
    for (const auto& token : tokens) {
      auto pair = results_trie_.insert(token, std::unordered_set<uint64_t>{});
      pair.first->insert(id);
    }
  }
}
