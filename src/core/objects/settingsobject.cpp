#include "settingsobject.h"

#include "../../ui/settingswindow.h"

SettingsObject::SettingsObject(FeatureModel* model) : FeatureObject{model} {}

void SettingsObject::Drag() {}

void SettingsObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      FeatureObject::ProcessKeyPress(combination);
      break;
    case Qt::Key_Return:
      emit Hidden();
      (new SettingsWindow{})->show();
      FeatureObject::ProcessKeyPress(combination);
      break;
    default:
      break;
  }
}

void SettingsObject::ProcessKeyRelease(const QKeyCombination& combination) {}
