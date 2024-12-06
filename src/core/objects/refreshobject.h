#pragma once

#include "featureobject.h"

class RefreshObject : public FeatureObject {
 public:
  explicit RefreshObject(FeatureModel* model);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;
};
