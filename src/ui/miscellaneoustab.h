#pragma once

#include <memory>

#include "toggletab.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MiscellaneousTab;
}
QT_END_NAMESPACE

class MiscellaneousTab : public ToggleTab {
  Q_OBJECT

 public:
  MiscellaneousTab(QWidget* parent = nullptr);

  ~MiscellaneousTab();

 public slots:
  void TranslateUi();

 private:
  std::unique_ptr<Ui::MiscellaneousTab> ui_;

 private slots:
  void ToggleRefreshModel(Qt::CheckState state);
  void ToggleTrashModel(Qt::CheckState state);
};
