#include "calculatormodel.h"

#include <memory>

#include "../bridges/calculatorbridge.h"
#include "../paths.h"
#include "../visitors/featurevisitor.h"

CalculatorModel::CalculatorModel()
    : FeatureModel{std::make_unique<CalculatorBridge>()} {
  SetIcon(Paths::GetPath(Paths::Image::kCalculator));
  SetId(16);
  SetTitlePlaceholder("...");
}

void CalculatorModel::Accept(FeatureVisitor& v) { v.Visit(this); }
