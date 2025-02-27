#pragma once

#include <QWidget>
#include <Qt>
#include <cstdint>
#include <memory>

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
  void ModelToggled(Qt::CheckState state, uint64_t id);

 private:
  std::unique_ptr<Ui::CalculatorTab> ui_;

 private slots:
  void ToggleCalculatorModel(Qt::CheckState state);
};
