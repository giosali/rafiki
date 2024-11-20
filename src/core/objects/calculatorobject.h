#pragma once

#include <string>

#include "../models/calculatormodel.h"
#include "featureobject.h"

class CalculatorObject final : public FeatureObject {
  Q_OBJECT

 public:
  explicit CalculatorObject(const CalculatorModel* model,
                            const std::string& value);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  QString FormatNumber(QString number) const;

  const CalculatorModel* model_{nullptr};
};
