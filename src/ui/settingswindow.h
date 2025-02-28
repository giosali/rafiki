#pragma once

#include <QMainWindow>
#include <Qt>
#include <cstdint>
#include <memory>

#include "calculatortab.h"
#include "filesearchtab.h"
#include "generaltab.h"
#include "miscellaneoustab.h"
#include "websearchestab.h"
#include "yourwebsearchestab.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SettingsWindow;
}
QT_END_NAMESPACE

class SettingsWindow : public QMainWindow {
  Q_OBJECT

 public:
  SettingsWindow(QWidget* parent = nullptr);

  ~SettingsWindow();

 private:
  std::unique_ptr<Ui::SettingsWindow> ui_;

 private slots:
  void ToggleModel(Qt::CheckState state, uint64_t id) const;
};
