#ifndef PROJECT_H
#define PROJECT_H

#include <QSettings>
#include <QString>
#include <memory>
#include <unordered_map>
#include <vector>

#include "autocompletemap.h"
#include "baseresult.h"
#include "definitions.h"
#include "input.h"

class Project {
 public:
  static std::vector<std::shared_ptr<BaseResult>> FindBaseResults(
    const Input& input);
  static QString GetDataFilePath(defs::DataFile file);
  static std::vector<std::shared_ptr<BaseResult>> GetDefaultBaseResults();
  static QString GetImageFilePath(defs::ImageFile file);
  static void Initialize();

 private:
  static void AddBaseResult(const std::shared_ptr<BaseResult> base_result);
  static QSettings GetDefaultSettings();
  static QSettings GetUserSettings();
  template <typename T>
  static void ParseJsonToBaseResults(const QString& path);
  static void SetMissingSettings(QSettings& internal_settings,
                                 QSettings& external_settings,
                                 const QString& group);
  static void UpdateDefaultBaseResults();

  static inline AutocompleteMap autocomplete_;
  static inline std::unordered_map<QString,
                                   std::vector<std::shared_ptr<BaseResult>>>
    base_results_map_;
  static inline std::vector<std::shared_ptr<BaseResult>> default_base_results_;
};

#endif  // PROJECT_H
