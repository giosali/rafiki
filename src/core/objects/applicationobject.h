#pragma once

#include "featureobject.h"

class ApplicationObject final : public FeatureObject {
  Q_OBJECT

 public:
  explicit ApplicationObject(FeatureModel* model);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;
};
