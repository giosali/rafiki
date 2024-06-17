#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include <memory>

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
};

#endif  // SETTINGSWINDOW_H
