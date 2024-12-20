#pragma once

#include <QCloseEvent>
#include <QMainWindow>
#include <QShowEvent>
#include <Qt>
#include <cstdint>
#include <memory>

#include "../core/models/featuremodel.h"
#include "filesearchtab.h"

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
  void DisableModel(FeatureModel* model) const;
  void EditWebSearch(bool checked) const;
  void EnableModel(FeatureModel* model) const;
  void SetEnabledButtons() const;
  void ToggleModel(Qt::CheckState state, uint64_t id) const;

 private:
  void ClearWebSearches() const;
  void LoadWebSearches() const;
  void OpenWebSearchDialog(uint64_t id) const;
  void SaveWebSearches() const;

  std::unique_ptr<Ui::SettingsWindow> ui_;
};
