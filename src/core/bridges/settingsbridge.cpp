#include "settingsbridge.h"

#include "../models/settingsmodel.h"
#include "../objects/settingsobject.h"

std::vector<FeatureObject*> SettingsBridge::ProcessInput(
  FeatureModel* feature_model, const QString& input) {
  auto model = static_cast<SettingsModel*>(feature_model);
  return {new SettingsObject{model}};
}
