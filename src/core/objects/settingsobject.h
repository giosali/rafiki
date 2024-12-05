#pragma once

#include "featureobject.h"

class SettingsObject : public FeatureObject {
 public:
  explicit SettingsObject(FeatureModel* model);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;
};
