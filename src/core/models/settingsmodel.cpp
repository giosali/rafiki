#include "settingsmodel.h"

#include <memory>

#include "../bridges/settingsbridge.h"
#include "../paths.h"
#include "../visitors/featurevisitor.h"

SettingsModel::SettingsModel()
    : FeatureModel{std::make_unique<SettingsBridge>()} {
  SetCommand("settings");
  SetIcon(Paths::GetPath(Paths::Image::kSettings));
  SetId(20);
  SetTitle(tr("Open Rafiki Settings"));
}

void SettingsModel::Accept(FeatureVisitor& v) { v.Visit(this); }

bool SettingsModel::ReceivesInput() const { return false; }
