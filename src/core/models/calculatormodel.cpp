#include "calculatormodel.h"

#include <memory>

#include "../bridges/calculatorbridge.h"
#include "../visitors/featurevisitor.h"

CalculatorModel::CalculatorModel()
    : FeatureModel{std::make_unique<CalculatorBridge>()} {
  SetIcon(":/icons/calculator.png");
  SetId(16);
  SetTitlePlaceholder("...");
}

void CalculatorModel::Accept(FeatureVisitor& v) { v.Visit(this); }
