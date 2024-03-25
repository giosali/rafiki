#ifndef PROJECTIO_H
#define PROJECTIO_H

#include <QSettings>
#include <QString>
#include <memory>
#include <unordered_map>
#include <vector>

#include "autocompletemap.h"
#include "baseresult.h"

class ProjectIO {
 public:
  enum class DataFile { kSettings, kWebSearches };
  enum class ImageFile { kCalculator, kQuestionMark };

  explicit ProjectIO();

  static QString GetDataFilePath(DataFile file);
  static QString GetImageFilePath(ImageFile file);
  std::vector<std::shared_ptr<BaseResult>> FindBaseResults(const QString& cmd);
  std::vector<std::shared_ptr<BaseResult>> GetDefaultBaseResults();

 private:
  template <typename T>
  void ParseDataFile(const QString& path);
  void PopulateDefaultBaseResults(QSettings& external_settings);
  void SetMissingSettings(QSettings& internal_settings,
                          QSettings& external_settings,
                          const QString& group) const;

  AutocompleteMap autocomplete_map_;
  std::unordered_map<QString, std::vector<std::shared_ptr<BaseResult>>>
    data_models_map_;
  std::vector<std::shared_ptr<BaseResult>> default_data_models_;
};

#endif  // PROJECTIO_H
