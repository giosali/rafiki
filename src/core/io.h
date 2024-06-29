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
#include "input.h"

class Io {
 public:
  enum class IniFile { kDefault, kUser };
  enum class ImageFile {
    kCalculator,
    kFile,
    kFileSystemEntry,
    kFolder,
    kQuestionMark,
    kTrash,
    kUrl
  };
  enum class JsonFile { kWebSearches, kYourWebSearches };

  Io() = delete;

  template <typename T>
  static std::vector<std::shared_ptr<T>> FilterResults() {
    auto results = std::vector<std::shared_ptr<T>>{};
    for (const auto& [_, result] : results_) {
      auto i = std::dynamic_pointer_cast<T>(result);
      if (i != nullptr) {
        results.push_back(i);
      }
    }

    return results;
  };
  static std::vector<std::shared_ptr<Result>> FindResults(const Input& input);
  static std::vector<std::shared_ptr<Result>> GetDefaultResults();
  static QString GetFilePath(ImageFile f);
  static QString GetIcon(const std::filesystem::path& path);
  static void Initialize();
  static void ToggleResult(uint64_t id, bool enable);

 private:
  static const QString kConfigDirectory;
  static const QString kDataDirectory;

  static void AddProcessedResult(
    const std::shared_ptr<ProcessedResult>& result);
  static void AddProcessedResultBuilder(
    const std::shared_ptr<ProcessedResultBuilder>& result);
  static void AddResult(const std::shared_ptr<Result>& result);
  static void AddResultHelper(const std::shared_ptr<Result>& result);
  static QSettings GetFile(IniFile f);
  static QJsonDocument GetFile(JsonFile f);
  static QString GetFilePath(IniFile f);
  static QString GetFilePath(JsonFile f);
  template <typename T>
  static void ParseJson(JsonFile f);
  static void UpdateDefaultResults();

  static Autocompleter autocompleter_;
  static std::vector<std::shared_ptr<Result>> default_results_;
  static std::unordered_set<uint64_t> disabled_ids_;
  static std::unordered_map<std::string, QString> mimetype_icons_;
  static std::vector<std::shared_ptr<ProcessedResultBuilder>>
    processed_result_builders_;
  static std::vector<std::shared_ptr<ProcessedResult>> processed_results_;
  static std::unordered_map<uint64_t, std::shared_ptr<Result>> results_;
  static std::unordered_map<QString, std::vector<std::shared_ptr<Result>>>
    results_map_;
};

#endif  // PROJECT_H
