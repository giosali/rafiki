#pragma once

#include "../models/urlmodel.h"
#include "featureobject.h"

class UrlObject final : public FeatureObject {
  Q_OBJECT

 public:
  explicit UrlObject(UrlModel* model, const QString& url);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;
};
