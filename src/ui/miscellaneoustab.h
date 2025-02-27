#pragma once

#include <QWidget>
#include <Qt>
#include <cstdint>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MiscellaneousTab;
}
QT_END_NAMESPACE

class MiscellaneousTab : public QWidget {
  Q_OBJECT

 public:
  MiscellaneousTab(QWidget* parent = nullptr);

  ~MiscellaneousTab();

 signals:
  void ModelToggled(Qt::CheckState state, uint64_t id);

 private:
  std::unique_ptr<Ui::MiscellaneousTab> ui_;

 private slots:
  void ToggleRefreshModel(Qt::CheckState state);
  void ToggleTrashModel(Qt::CheckState state);
};
