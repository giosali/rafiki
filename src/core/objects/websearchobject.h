#pragma once

#include "../models/websearchmodel.h"
#include "featureobject.h"

class WebSearchObject final : public FeatureObject {
  Q_OBJECT

 public:
  WebSearchObject(const WebSearchModel* model, const QString& command,
                  const QString& argument);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  QString argument_{};
  QString command_{};
  const WebSearchModel* model_{nullptr};
};
