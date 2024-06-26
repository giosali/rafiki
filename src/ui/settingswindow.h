#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QShowEvent>
#include <QUuid>
#include <cstdint>
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

 protected:
  void closeEvent(QCloseEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  void ProcessStateChange(int state, uint64_t id) const;

 private:
  std::unique_ptr<Ui::SettingsWindow> ui_;
};

#endif  // SETTINGSWINDOW_H
