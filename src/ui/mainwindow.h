#pragma once

#include <QEvent>
#include <QKeyCombination>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTranslator>
#include <memory>

#include "../core/theme.h"
#include "searchbox.h"  // Necessary

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
  void ToggleVisibility();

 signals:
  void Deactivated();
  void TextReceived(const QString& text);

 protected:
  void changeEvent(QEvent* event) override;
  bool event(QEvent* event) override;

 private:
  QTranslator language_translator_{};
  QTranslator territory_translator_{};
  std::unique_ptr<Ui::MainWindow> ui_;

 private slots:
  void ApplyTheme(Theme* theme);
  void Hide();
  void OpenSettingsWindow();
  void ProcessActivationReason(QSystemTrayIcon::ActivationReason reason);
  void ProcessKeyPress(const QKeyCombination& combination);
  void SetHeight(int height);
  void UpdateTranslators();
};
