#pragma once

#include "featureobject.h"

class WebSearchObject final : public FeatureObject {
  Q_OBJECT

 public:
  WebSearchObject(const FeatureModel* model, const QString& input,
                  const QString& argument);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  QString argument_{};
};
