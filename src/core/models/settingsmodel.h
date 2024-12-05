#pragma once

#include <QCoreApplication>

#include "featuremodel.h"

class SettingsModel : public FeatureModel {
  Q_DECLARE_TR_FUNCTIONS(SettingsModel)

 public:
  SettingsModel();

  void Accept(FeatureVisitor& v) override;
  bool ReceivesInput() const override;
};
