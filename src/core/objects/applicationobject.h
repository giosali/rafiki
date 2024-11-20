#pragma once

#include <QString>

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
  QString input_{};
  const ApplicationModel* model_{nullptr};
};
