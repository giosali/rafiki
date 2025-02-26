#pragma once

#include <QMainWindow>
#include <Qt>
#include <cstdint>
#include <memory>

#include "../core/models/featuremodel.h"
#include "calculatortab.h"
#include "filesearchtab.h"
#include "generaltab.h"
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

 public slots:
  void DisableModel(FeatureModel* model) const;
  void EnableModel(FeatureModel* model) const;
  void ToggleModel(Qt::CheckState state, uint64_t id) const;

 private:
  std::unique_ptr<Ui::SettingsWindow> ui_;
};
