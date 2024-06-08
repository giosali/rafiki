#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QKeyCombination>
#include <QString>

#include "../core/input.h"
#include "processedresult.h"

class Calculator : public ProcessedResult {
  Q_OBJECT

 public:
  Calculator();

  bool ProcessInput(const Input& input) override;

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  static const QString kAltTitle;
  static const bool kAppendSpaceToCommand;
  static const QString kCommand;
  static const QString kDescription;
  static const QString kIcon;
  static const QString kId;
  static const QString kInfoDescription;
  static const QString kTitle;
  static const QString kTitlePlaceholder;

  QString FormatNumber(QString number) const;
};

#endif  // CALCULATOR_H
