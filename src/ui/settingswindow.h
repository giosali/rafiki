#pragma once

#include <QMainWindow>
#include <Qt>
#include <cstdint>
#include <memory>

#include "calculatortab.h"       // Necessary
#include "filesearchtab.h"       // Necessary
#include "generaltab.h"          // Necessary
#include "miscellaneoustab.h"    // Necessary
#include "websearchestab.h"      // Necessary
#include "yourwebsearchestab.h"  // Necessary

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
