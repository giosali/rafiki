#pragma once

#include <QWidget>
#include <Qt>
#include <memory>

#include "../core/models/featuremodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class FileSearchTab;
}
QT_END_NAMESPACE

class FileSearchTab : public QWidget {
  Q_OBJECT

 public:
  FileSearchTab(QWidget* parent = nullptr);

  ~FileSearchTab();

 signals:
  void ModelDisabled(FeatureModel* model);
  void ModelEnabled(FeatureModel* model);

 private:
  std::unique_ptr<Ui::FileSearchTab> ui_;

 private slots:
  void ToggleFileSystemEntryModel(Qt::CheckState state);
  void UpdateIgnoredDirectoryNames();
};
