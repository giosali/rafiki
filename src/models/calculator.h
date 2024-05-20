#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QKeyCombination>
#include <QString>

#include "../core/definitions.h"
#include "../core/input.h"
#include "../core/project.h"
#include "processedresult.h"

class Calculator : public ProcessedResult {
 public:
  Calculator();

  QString DragAndDrop() override;
  bool ProcessInput(const Input& input) override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  static const inline QString kAltTitle{};
  static const bool kAppendSpaceToCommand = false;
  static const inline QString kCommand{};
  static const inline QString kDescription{"Copy to clipboard"};
  static const inline QString kIcon{
    Project::GetImageFilePath(defs::ImageFile::kCalculator)};
  static const inline QString kId{"7cddc690-1b3d-49d5-bbe4-adbad5a51833"};
  static const inline QString kInfoDescription{
    "Please enter a valid expression"};
  static inline QString kTitle{};
  static const inline QString kTitlePlaceholder{"..."};

  QString FormatNumber(QString number) const;
};

#endif  // CALCULATOR_H
