#ifndef SETTINGS_H
#define SETTINGS_H

#include <QJsonDocument>
#include <QString>
#include <cstdint>
#include <unordered_set>
#include <vector>

class Settings {
 public:
  static Settings& GetInstance();
  std::vector<uint64_t> GetDefaultFeatureModelIds() const;
  std::unordered_set<uint64_t> GetDisabledFeatureModelids() const;
  int GetSearchResultIconSize() const;
  int GetSearchResultListMaxCount() const;
  int GetSearchResultTitleMaxHeight() const;
  void Save(const QString& path) const;
  void ToggleDisabledFeatureModelId(uint64_t id);
  void Update(const QJsonDocument& document);

 private:
  Settings() = default;
  Settings(const Settings&) = delete;

  Settings operator=(const Settings&) = delete;

  std::vector<uint64_t> default_feature_model_ids_{1, 2, 3};
  std::unordered_set<uint64_t> disabled_feature_model_ids_{};
  int search_result_icon_size_{44};
  int search_result_list_max_count_{6};
  int search_result_title_max_height_{25};
};

#endif  // SETTINGS_H
