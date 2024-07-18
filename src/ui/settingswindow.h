#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QShowEvent>
#include <cstdint>
#include <memory>

#include "../core/id.h"

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

 public slots:
  void AddWebSearch(bool checked) const;
  void DeleteWebSearch(bool checked) const;
  void EditWebSearch(bool checked) const;
  void SetEnabledButtons() const;
  void ToggleResult(int state, const Id& id) const;
  void ToggleStartup(int state) const;

 protected:
  void closeEvent(QCloseEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  void ClearWebSearches() const;
  void LoadWebSearches() const;
  void OpenWebSearchDialog(const Id& id) const;

 private:
  std::unique_ptr<Ui::SettingsWindow> ui_;
};

#endif  // SETTINGSWINDOW_H
