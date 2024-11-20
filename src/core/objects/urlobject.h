#pragma once

#include "../models/urlmodel.h"
#include "featureobject.h"

class UrlObject final : public FeatureObject {
  Q_OBJECT

 public:
  explicit UrlObject(const UrlModel* model, const QString& url);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  const UrlModel* model_{nullptr};
};
