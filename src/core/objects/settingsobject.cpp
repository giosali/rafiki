#include "settingsobject.h"

SettingsObject::SettingsObject(FeatureModel* model) : FeatureObject{model} {}

void SettingsObject::Drag() {}

void SettingsObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      break;
    case Qt::Key_Return:
      emit Hidden();
      break;
    default:
      break;
  }
}

void SettingsObject::ProcessKeyRelease(const QKeyCombination& combination) {}
