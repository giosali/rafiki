#pragma once

#include "../models/applicationmodel.h"
#include "featureobject.h"

class ApplicationObject final : public FeatureObject {
  Q_OBJECT

 public:
  explicit ApplicationObject(const ApplicationModel* model);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  const ApplicationModel* model_{nullptr};
};
