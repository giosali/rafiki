#ifndef PROJECT_H
#define PROJECT_H

#include <QSettings>
#include <QString>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../models/baseresult.h"
#include "../models/processedresult.h"
#include "../models/processedresultbuilder.h"
#include "autocompletemap.h"
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
    kTrash
  };

  Io() = delete;

  static std::vector<std::shared_ptr<BaseResult>> FindBaseResults(
    const Input& input);
  static std::vector<std::shared_ptr<BaseResult>> GetDefaultBaseResults();
  static QString GetIcon(ImageFile file);
  static QString GetIcon(const std::filesystem::path& path);
  static void Initialize();

 private:
  static void AddBaseResult(const std::shared_ptr<BaseResult>& base_result);
  static void AddProcessedBaseResult(
    const std::shared_ptr<ProcessedResult>& processed_result);
  static void AddProcessedResultBuilder(
    const std::shared_ptr<ProcessedResultBuilder>& builder);
  static QSettings GetFile(ConfigFile file);
  static QString GetFile(DataFile file);
  template <typename T>
  static void ParseJsonToBaseResults(const QString& path);
  static void UpdateDefaultBaseResults();

  static inline AutocompleteMap autocomplete_;
  static inline std::unordered_map<QString,
                                   std::vector<std::shared_ptr<BaseResult>>>
    base_results_map_;
  static inline std::vector<std::shared_ptr<BaseResult>> default_base_results_;
  static inline std::vector<std::shared_ptr<ProcessedResult>>
    processed_base_results_;
  static inline std::vector<std::shared_ptr<ProcessedResultBuilder>>
    processed_result_builders_;
  static inline std::unordered_map<std::string, QString> mimetype_icons_;
};

#endif  // PROJECT_H
