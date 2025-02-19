#pragma once

#include <QJsonDocument>
#include <QLocale>
#include <QObject>
#include <QString>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Settings : public QObject {
  Q_OBJECT

 public:
  static Settings& GetInstance();
  void AddDisabledFeatureModelId(uint64_t id);
  void AddIgnoredDirectoryName(const std::string& name);
  void AddUseCount(uint64_t id, uint64_t count);
  void ClearIgnoredDirectoryNames();
  uint64_t GetAvailableId() const;
  std::vector<uint64_t> GetDefaultFeatureModelIds() const;
  std::unordered_set<uint64_t> GetDisabledFeatureModelids() const;
  std::unordered_set<std::string> GetIgnoredDirectoryNames() const;
  QLocale::Language GetLanguage() const;
  int GetSearchResultIconSize() const;
  int GetSearchResultListMaxCount() const;
  int GetSearchResultTitleMaxHeight() const;
  QLocale::Territory GetTerritory() const;
  QString GetThemeFilename() const;
  std::unordered_map<uint64_t, uint64_t> GetUseCounts() const;
  void Initialize();
  void RemoveDisabledFeatureModelId(uint64_t id);
  void RemoveUseCount(uint64_t id);
  void Save() const;
  void SetAvailableId(uint64_t value);
  void SetLanguage(QLocale::Language value);
  void SetTerritory(QLocale::Territory value);
  void SetThemeFilename(const QString& value);
  void Update(const QJsonDocument& document);

 signals:
  void LocaleChanged();

 private:
  Settings() = default;
  Settings(const Settings&) = delete;

  Settings operator=(const Settings&) = delete;

  uint64_t available_id_{0x8000};  // WM_APP
  std::vector<uint64_t> default_feature_model_ids_{1, 2, 3};
  std::unordered_set<uint64_t> disabled_feature_model_ids_{};
  std::unordered_set<std::string> ignored_directory_names_{"node_modules",
                                                           "site-packages"};
  QLocale::Language language_{QLocale::Language::English};
  int search_result_icon_size_{44};
  int search_result_list_max_count_{6};
  int search_result_title_max_height_{25};
  QLocale::Territory territory_{QLocale::Territory::UnitedStates};
  QString theme_filename_{"dark-theme.json"};
  std::unordered_map<uint64_t, uint64_t> use_counts_{};
};
