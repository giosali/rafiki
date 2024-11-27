#include "settings.h"

#include <QJsonArray>
#include <QJsonObject>

#include "file.h"
#include "paths.h"

Settings& Settings::GetInstance() {
  // Thread-safe magic static.
  static auto instance = Settings{};
  return instance;
}

void Settings::AddDisabledFeatureModelId(uint64_t id) {
  disabled_feature_model_ids_.insert(id);
}

uint64_t Settings::GetAvailableId() const { return available_id_; }

std::vector<uint64_t> Settings::GetDefaultFeatureModelIds() const {
  return default_feature_model_ids_;
}

std::unordered_set<uint64_t> Settings::GetDisabledFeatureModelids() const {
  return disabled_feature_model_ids_;
}

int Settings::GetSearchResultIconSize() const {
  return search_result_icon_size_;
}

int Settings::GetSearchResultListMaxCount() const {
  return search_result_list_max_count_;
}

int Settings::GetSearchResultTitleMaxHeight() const {
  return search_result_title_max_height_;
}

void Settings::RemoveDisabledFeatureModelId(uint64_t id) {
  disabled_feature_model_ids_.erase(id);
}

void Settings::Save() const {
  auto object = QJsonObject{};

  // "defaultModels"
  auto default_models = QJsonArray{};
  for (auto id : default_feature_model_ids_) {
    default_models.append(QString::number(id));
  }

  object.insert("defaultModels", default_models);

  // "disabledModels"
  auto disabled_models = QJsonArray{};
  for (auto id : disabled_feature_model_ids_) {
    disabled_models.append(QString::number(id));
  }

  object.insert("disabledModels", disabled_models);

  // "availableId"
  object.insert("availableId", QString::number(available_id_));

  File::Write(Paths::GetPath(Paths::Json::kUserSettings), object);
}

void Settings::SetAvailableId(uint64_t value) { available_id_ = value; }

void Settings::Update(const QJsonDocument& document) {
  auto object = document.object();
  if (object.isEmpty()) {
    return;
  }

  if (auto it = object.find("defaultModels"); it != object.end()) {
    default_feature_model_ids_.clear();
    for (auto value : it.value().toArray()) {
      default_feature_model_ids_.push_back(value.toString().toULongLong());
    }
  }

  if (auto it = object.find("disabledModels"); it != object.end()) {
    disabled_feature_model_ids_.clear();
    for (auto value : it.value().toArray()) {
      disabled_feature_model_ids_.insert(value.toString().toULongLong());
    }
  }

  if (auto it = object.find("availableId"); it != object.end()) {
    available_id_ = it.value().toString().toULongLong();
  }
}
