#pragma once

#include <QEvent>
#include <QMainWindow>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QTranslator>
#include <memory>

#include "searchbox.h"

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
  void Show();

 public slots:
  void ProcessCommandLineArguments(const QStringList& arguments,
                                   bool from_server);

 signals:
  void Deactivated();
  void TextReceived(const QString& text);

 protected:
  void changeEvent(QEvent* event) override;
  bool event(QEvent* event) override;

 private:
  void ToggleVisibility();

  QTranslator language_translator_{};
  QTranslator territory_translator_{};
  std::unique_ptr<Ui::MainWindow> ui_;

 private slots:
  void Hide();
  void OpenSettingsWindow();
  void ProcessActivationReason(QSystemTrayIcon::ActivationReason reason);
  void SetHeight(int height);
  void UpdateTranslators();
};
