#pragma once

#include <QObject>
#include <QTest>
#include <memory>

#include "core/bridges/calculatorbridge.h"
#include "core/models/calculatormodel.h"

class TestCalculatorBridge : public QObject {
  Q_OBJECT

 public:
  TestCalculatorBridge();

 private:
  CalculatorBridge bridge_{};
  std::unique_ptr<CalculatorModel> model_{nullptr};

 private slots:
  void ProcessInput_Addition();
  void ProcessInput_Addition_data();
  void ProcessInput_Decimals();
  void ProcessInput_Decimals_data();
  void ProcessInput_Division();
  void ProcessInput_Division_data();
  void ProcessInput_Exponentiation();
  void ProcessInput_Exponentiation_data();
  void ProcessInput_Factorial();
  void ProcessInput_Factorial_data();
  void ProcessInput_Incomplete();
  void ProcessInput_Incomplete_data();
  void ProcessInput_Invalid();
  void ProcessInput_Invalid_data();
  void ProcessInput_Multiplication();
  void ProcessInput_Multiplication_data();
  void ProcessInput_Negatives();
  void ProcessInput_Negatives_data();
  void ProcessInput_Numbers();
  void ProcessInput_Numbers_data();
  void ProcessInput_Parentheses();
  void ProcessInput_Parentheses_data();
  void ProcessInput_Pemdas();
  void ProcessInput_Pemdas_data();
  void ProcessInput_Subtraction();
  void ProcessInput_Subtraction_data();
};
