#pragma once

#include "featuremodel.h"

class TrashModel final : public FeatureModel {
 public:
  TrashModel();

  void Accept(FeatureVisitor& v) override;
  std::unordered_set<std::string> Tokenize() const override;
};
