#pragma once

#include <memory>

#include "toggletab.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CalculatorTab;
}
QT_END_NAMESPACE

class CalculatorTab : public ToggleTab {
  Q_OBJECT

 public:
  CalculatorTab(QWidget* parent = nullptr);

  ~CalculatorTab();

 public slots:
  void TranslateUi();

 private:
  std::unique_ptr<Ui::CalculatorTab> ui_;

 private slots:
  void ToggleCalculatorModel(Qt::CheckState state);
};
