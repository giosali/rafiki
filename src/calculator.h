#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QString>

#include "baseresult.h"
#include "definitions.h"
#include "input.h"
#include "project.h"

class Calculator : public BaseResult {
 public:
  Calculator();

  QString DragAndDrop() override;
  bool ProcessInput(const Input& input) override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       QWidget* parent) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         QWidget* parent) override;

 private:
  static const inline QString kAltTitle{};
  static const inline QString kCommand{};
  static const inline QString kDescription{"Copy to clipboard"};
  static const inline QString kIcon{
    Project::GetImageFilePath(defs::ImageFile::kCalculator)};
  static const inline QString kId{"7cddc690-1b3d-49d5-bbe4-adbad5a51833"};
  static const inline QString kInfoDescription{
    "Please enter a valid expression"};
  static const inline QString kPlaceholder{"..."};
  static const inline QString kTitle{};
};

#endif  // CALCULATOR_H
