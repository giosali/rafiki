#pragma once

#include <QWidget>
#include <Qt>
#include <memory>

#include "../core/models/featuremodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CalculatorTab;
}
QT_END_NAMESPACE

class CalculatorTab : public QWidget {
  Q_OBJECT

 public:
  CalculatorTab(QWidget* parent = nullptr);

  ~CalculatorTab();

 signals:
  void ModelDisabled(FeatureModel* model);
  void ModelEnabled(FeatureModel* model);

 private:
  std::unique_ptr<Ui::CalculatorTab> ui_;

 private slots:
  void ToggleCalculatorModel(Qt::CheckState state);
};
