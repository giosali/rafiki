#pragma once

#include "featuremodel.h"

class UrlModel final : public FeatureModel {
 public:
  UrlModel();

  void Accept(FeatureVisitor& v) override;
};
