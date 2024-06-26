#ifndef PROJECT_H
#define PROJECT_H

#include <QSettings>
#include <QString>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../models/processedresult.h"
#include "../models/processedresultbuilder.h"
#include "../models/result.h"
#include "../models/websearch.h"
#include "autocompleter.h"
#include "input.h"

class Io {
 public:
  enum class ConfigFile { kDefault, kUser };
  enum class DataFile { kSettings, kWebSearches };
  enum class ImageFile {
    kCalculator,
    kFile,
    kFileSystemEntry,
    kFolder,
    kQuestionMark,
    kTrash,
    kUrl
  };

  Io() = delete;

  static std::vector<std::shared_ptr<Result>> FindResults(const Input& input);
  static std::vector<std::shared_ptr<Result>> GetDefaultResults();
  static QString GetIcon(ImageFile file);
  static QString GetIcon(const std::filesystem::path& path);
  static std::vector<std::shared_ptr<WebSearch>> GetWebSearches();
  static void Initialize();

 private:
  static void AddResult(const std::shared_ptr<Result>& result);
  static void AddProcessedResult(
    const std::shared_ptr<ProcessedResult>& processed_result);
  static void AddProcessedResultBuilder(
    const std::shared_ptr<ProcessedResultBuilder>& builder);
  static QSettings GetFile(ConfigFile file);
  static QString GetFile(DataFile file);
  template <typename T>
  static void ParseJsonToResults(const QString& path);
  static void UpdateDefaultResults();

  static inline Autocompleter autocompleter_;
  static inline std::unordered_map<QString,
                                   std::vector<std::shared_ptr<Result>>>
    results_map_;
  static inline std::vector<std::shared_ptr<Result>> default_results_;
  static inline std::vector<std::shared_ptr<ProcessedResult>>
    processed_results_;
  static inline std::vector<std::shared_ptr<ProcessedResultBuilder>>
    processed_result_builders_;
  static inline std::unordered_map<std::string, QString> mimetype_icons_;
};

#endif  // PROJECT_H
