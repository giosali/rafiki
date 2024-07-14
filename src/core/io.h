#ifndef PROJECT_H
#define PROJECT_H

#include <QJsonDocument>
#include <QSettings>
#include <QString>
#include <filesystem>
#include <memory>
#include <string>
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

class Io {
 public:
  enum class Directory { kYourIcons };
  enum class Ini { kDefault, kUser };
  enum class Image {
    kCalculator,
    kFile,
    kFileSystemEntry,
    kFolder,
    kQuestionMark,
    kTrash,
    kUrl
  };
  enum class Json { kWebSearches, kYourWebSearches };

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
  static QString GetDirectoryPath(Directory directory);
  static QString GetFilePath(Image f);
  static QString GetIcon(const std::filesystem::path& path);
  static void Initialize();
  static void ToggleResult(const Id& id, bool enable);

 private:
  static const QString kDataDirectory;

  static void AddProcessedResult(
    const std::shared_ptr<ProcessedResult>& result);
  static void AddProcessedResultBuilder(
    const std::shared_ptr<ProcessedResultBuilder>& result);
  static void AddResult(const std::shared_ptr<Result>& result);
  static void AddResultHelper(const std::shared_ptr<Result>& result);
  static QSettings GetFile(Ini f);
  static QJsonDocument GetFile(Json f);
  static QString GetFilePath(Ini f);
  static QString GetFilePath(Json f);
  template <typename T>
  static void ParseJson(Json f);
  static void RemoveResult(const std::shared_ptr<Result>& result);
  static void SaveYourWebSearches();
  static void SaveYourWebSearches(const QJsonDocument& document);
  static void UpdateDefaultResults();

  static Autocompleter autocompleter_;
  static QString config_directory_;
  static std::vector<std::shared_ptr<Result>> default_results_;
  static std::unordered_set<Id> disabled_ids_;
  static std::unordered_map<std::string, QString> mimetype_icons_;
  static std::vector<std::shared_ptr<ProcessedResultBuilder>>
    processed_result_builders_;
  static std::vector<std::shared_ptr<ProcessedResult>> processed_results_;
  static std::unordered_map<Id, std::shared_ptr<Result>> results_;
  static std::unordered_map<QString, std::vector<std::shared_ptr<Result>>>
    results_map_;
};

#endif  // PROJECT_H
