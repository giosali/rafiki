#ifndef CRUD_H
#define CRUD_H

#include <QString>
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
#include "paths.h"

class Crud {
 public:
  Crud() = delete;

  static void CreateIcon(const std::shared_ptr<Result>& result);
  static void CreateWebSearch(const std::shared_ptr<WebSearch>& web_search);
  static void DeleteWebSearch(const Id& id);
  static void Initialize();
  static std::vector<std::shared_ptr<Result>> ReadDefaultResults();
  static std::shared_ptr<Result> ReadResult(const Id& id);
  template <typename T>
  static std::vector<std::shared_ptr<T>> ReadResults() {
    auto results = std::vector<std::shared_ptr<T>>{};
    for (const auto& [_, result] : results_) {
      if (auto i = std::dynamic_pointer_cast<T>(result); i != nullptr) {
        results.push_back(i);
      }
    }

    return results;
  };
  static std::vector<std::shared_ptr<Result>> ReadResults(const Input& input);
  static void ToggleDesktopEntry(bool create);
  static void ToggleResult(const Id& id, bool enable);
  static void UpdateWebSearch(const std::shared_ptr<WebSearch>& web_search,
                              const QString& command);

 private:
  static void CreateProcessedResult(
    const std::shared_ptr<ProcessedResult>& result);
  static void CreateProcessedResultBuilder(
    const std::shared_ptr<ProcessedResultBuilder>& result);
  static void CreateResult(const std::shared_ptr<Result>& result);
  template <typename T>
  static void CreateResult(Paths::Json f);
  static void CreateResultHelper(const std::shared_ptr<Result>& result);
  static void DeleteResult(const std::shared_ptr<Result>& result);
  static void UpdateYourWebSearches();
  static void UpdateYourWebSearches(const QJsonDocument& document);
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

#endif  // CRUD_H
