#pragma once

#include <QCoreApplication>

#include "featuremodel.h"

class RefreshModel final : public FeatureModel {
  Q_DECLARE_TR_FUNCTIONS(RefreshModel)

 public:
  RefreshModel();

  void Accept(FeatureVisitor& v) override;
  bool ReceivesInput() const override;
};
