#pragma once

#include <QEvent>
#include <QMainWindow>
#include <QStringList>
#include <QSystemTrayIcon>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);

  ~MainWindow();

  void CreateTrayIcon();

 public slots:
  void Hide();
  void OpenSettingsWindow();
  void ProcessActivationReason(QSystemTrayIcon::ActivationReason reason);
  void ProcessCommandLineArguments(const QStringList& arguments,
                                   bool from_server);
  void SetHeight(int height);

 signals:
  void Deactivated();

 protected:
  bool event(QEvent* event) override;

 private:
  void ToggleVisibility();

  std::unique_ptr<Ui::MainWindow> ui_;
};
