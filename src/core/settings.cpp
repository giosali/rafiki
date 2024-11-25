#include "settings.h"

#include <QJsonArray>
#include <QJsonObject>

#include "file.h"

Settings& Settings::GetInstance() {
  // Thread-safe magic static.
  static auto instance = Settings{};
  return instance;
}

std::vector<uint64_t> Settings::GetDefaultFeatureModelIds() const {
  return default_feature_model_ids_;
}

std::unordered_set<uint64_t> Settings::GetDisabledFeatureModelids() const {
  return disabled_feature_model_ids_;
}

bool Settings::GetRunOnStartup() const { return run_on_startup_; }

void Settings::Save(const QString& path) const {
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

  // "runOnStartup"
  object.insert("runOnStartup", run_on_startup_);

  File::Write(path, object);
}

void Settings::ToggleDisabledFeatureModelId(uint64_t id) {
  if (disabled_feature_model_ids_.contains(id)) {
    disabled_feature_model_ids_.erase(id);
  } else {
    disabled_feature_model_ids_.insert(id);
  }
}

void Settings::Update(const QJsonDocument& document) {
  auto object = document.object();
  if (object.isEmpty()) {
    return;
  }

  if (auto it = object.find("defaultModels"); it != object.end()) {
    auto array = it.value().toArray();

    auto temp_ids = std::vector<uint64_t>{};
    temp_ids.reserve(array.size());
    for (auto value : array) {
      temp_ids.push_back(value.toInt());
    }

    default_feature_model_ids_ = temp_ids;
  }

  if (auto it = object.find("runOnStartup"); it != object.end()) {
    if (auto value = it.value(); value.isBool()) {
      run_on_startup_ = value.toBool();
    }
  }
}
