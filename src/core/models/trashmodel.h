#pragma once

#include <QCoreApplication>

#include "featuremodel.h"

class TrashModel final : public FeatureModel {
  Q_DECLARE_TR_FUNCTIONS(TrashModel)

 public:
  TrashModel();

  void Accept(FeatureVisitor& v) override;
  bool ReceivesInput() const override;
  std::unordered_set<std::string> Tokenize() const override;
};
