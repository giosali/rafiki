#include "settingsmodel.h"

#include <memory>

#include "../bridges/settingsbridge.h"
#include "../visitors/featurevisitor.h"

SettingsModel::SettingsModel()
    : FeatureModel{std::make_unique<SettingsBridge>()} {
  SetCommand("settings");
  SetIcon(":/icons/settings.svg");
  SetId(20);
  SetTitle(tr("Open Rafiki settings"));
}

void SettingsModel::Accept(FeatureVisitor& v) { v.Visit(this); }

bool SettingsModel::ReceivesInput() const { return false; }
