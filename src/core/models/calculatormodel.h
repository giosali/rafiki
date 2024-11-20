#pragma once

#include <QString>

#include "featuremodel.h"

class CalculatorModel final : public FeatureModel {
 public:
  CalculatorModel();

  void Accept(FeatureVisitor& v) override;
};
