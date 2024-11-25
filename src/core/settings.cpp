#include "settings.h"

#include <QJsonArray>
#include <QJsonObject>

Settings& Settings::GetInstance() {
  // Thread-safe magic static.
  static auto instance = Settings{};
  return instance;
}

std::vector<uint64_t> Settings::GetDefaultFeatureModelIds() const {
  return default_feature_model_ids_;
}

bool Settings::GetRunOnStartup() const { return run_on_startup_; }

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
