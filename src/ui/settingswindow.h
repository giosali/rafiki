#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QShowEvent>
#include <Qt>
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

 public slots:
  void AddWebSearch(bool checked) const;
  void DeleteWebSearch(bool checked) const;
  void EditWebSearch(bool checked) const;
  void SetEnabledButtons() const;
  void ToggleModel(Qt::CheckState state, uint64_t id) const;

 protected:
  void closeEvent(QCloseEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  void ClearWebSearches() const;
  void LoadWebSearches() const;
  void OpenWebSearchDialog(uint64_t id) const;
  void SaveWebSearches() const;

  std::unique_ptr<Ui::SettingsWindow> ui_;
};

#endif  // SETTINGSWINDOW_H
