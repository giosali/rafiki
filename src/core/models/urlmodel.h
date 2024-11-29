#pragma once

#include <QCoreApplication>

#include "featuremodel.h"

class UrlModel final : public FeatureModel {
  Q_DECLARE_TR_FUNCTIONS(UrlModel)

 public:
  UrlModel();

  void Accept(FeatureVisitor& v) override;
};
