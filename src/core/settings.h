#pragma once

#include <QJsonDocument>
#include <QString>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Settings {
 public:
  static Settings& GetInstance();
  void AddDisabledFeatureModelId(uint64_t id);
  void AddUseCount(uint64_t id, uint64_t count);
  uint64_t GetAvailableId() const;
  std::vector<uint64_t> GetDefaultFeatureModelIds() const;
  std::unordered_set<uint64_t> GetDisabledFeatureModelids() const;
  QString GetLocale() const;
  int GetSearchResultIconSize() const;
  int GetSearchResultListMaxCount() const;
  int GetSearchResultTitleMaxHeight() const;
  std::unordered_map<uint64_t, uint64_t> GetUseCounts() const;
  void RemoveDisabledFeatureModelId(uint64_t id);
  void RemoveUseCount(uint64_t id);
  void Save() const;
  void SetAvailableId(uint64_t value);
  void Update(const QJsonDocument& document);

 private:
  Settings() = default;
  Settings(const Settings&) = delete;

  Settings operator=(const Settings&) = delete;

  uint64_t available_id_{0x8000};  // WM_APP
  std::vector<uint64_t> default_feature_model_ids_{1, 2, 3};
  std::unordered_set<uint64_t> disabled_feature_model_ids_{};
  QString locale_{"en_us"};
  int search_result_icon_size_{44};
  int search_result_list_max_count_{6};
  int search_result_title_max_height_{25};
  std::unordered_map<uint64_t, uint64_t> use_counts_{};
};
