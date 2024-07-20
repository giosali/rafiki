#ifndef PROJECT_H
#define PROJECT_H

#include <QJsonDocument>
#include <QSettings>
#include <QString>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../models/processedresult.h"
#include "../models/processedresultbuilder.h"
#include "../models/result.h"
#include "../models/websearch.h"
#include "autocompleter.h"
#include "id.h"
#include "input.h"
#include "paths.h"

class Io {
 public:
  Io() = delete;

  static void AddIcon(const std::shared_ptr<Result>& result);
  static void AddWebSearch(const std::shared_ptr<WebSearch>& web_search);
  static void DeleteWebSearch(const Id& id);
  static void EditWebSearch(const std::shared_ptr<WebSearch>& web_search,
                            const QString& command);
  template <typename T>
  static std::vector<std::shared_ptr<T>> FilterResults() {
    auto results = std::vector<std::shared_ptr<T>>{};
    for (const auto& [_, result] : results_) {
      if (auto i = std::dynamic_pointer_cast<T>(result); i != nullptr) {
        results.push_back(i);
      }
    }

    return results;
  };
  static std::shared_ptr<Result> FindResult(const Id& id);
  static std::vector<std::shared_ptr<Result>> FindResults(const Input& input);
  static std::vector<std::shared_ptr<Result>> GetDefaultResults();
  static void Initialize();
  static void ToggleResult(const Id& id, bool enable);
  static void ToggleDesktopEntry(bool create);

 private:
  static void AddProcessedResult(
    const std::shared_ptr<ProcessedResult>& result);
  static void AddProcessedResultBuilder(
    const std::shared_ptr<ProcessedResultBuilder>& result);
  static void AddResult(const std::shared_ptr<Result>& result);
  static void AddResultHelper(const std::shared_ptr<Result>& result);
  static QSettings GetFile(Paths::Ini f);
  static QJsonDocument GetFile(Paths::Json f);
  template <typename T>
  static void ParseJson(Paths::Json f);
  static void RemoveResult(const std::shared_ptr<Result>& result);
  static void SaveYourWebSearches();
  static void SaveYourWebSearches(const QJsonDocument& document);
  static void UpdateDefaultResults();

  static Autocompleter autocompleter_;
  static std::vector<std::shared_ptr<Result>> default_results_;
  static std::unordered_set<Id> disabled_ids_;
  static std::vector<std::shared_ptr<ProcessedResultBuilder>>
    processed_result_builders_;
  static std::vector<std::shared_ptr<ProcessedResult>> processed_results_;
  static std::unordered_map<Id, std::shared_ptr<Result>> results_;
  static std::unordered_map<QString, std::vector<std::shared_ptr<Result>>>
    results_map_;
};

#endif  // PROJECT_H
