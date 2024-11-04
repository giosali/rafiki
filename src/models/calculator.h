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
  static const QString kDescription;

  QString FormatNumber(QString number) const;
};

#endif  // CALCULATOR_H
