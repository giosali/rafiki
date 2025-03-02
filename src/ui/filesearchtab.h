#pragma once

#include <memory>

#include "toggletab.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class FileSearchTab;
}
QT_END_NAMESPACE

class FileSearchTab : public ToggleTab {
  Q_OBJECT

 public:
  FileSearchTab(QWidget* parent = nullptr);

  ~FileSearchTab();

 public slots:
  void TranslateUi();

 private:
  std::unique_ptr<Ui::FileSearchTab> ui_;

 private slots:
  void ToggleFileSystemEntryModel(Qt::CheckState state);
  void UpdateIgnoredDirectoryNames();
};
