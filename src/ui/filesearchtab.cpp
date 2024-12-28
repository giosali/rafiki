#include "filesearchtab.h"

#include <QCheckBox>
#include <QTextEdit>

#include "../core/indexer.h"
#include "../core/models/filesystementrymodel.h"
#include "../core/settings.h"
#include "./ui_filesearchtab.h"

FileSearchTab::FileSearchTab(QWidget* parent)
    : QWidget{parent}, ui_{std::make_unique<Ui::FileSearchTab>()} {
  ui_->setupUi(this);

  // Checks or unchecks QCheckBox control.
  if (auto models = Indexer::GetInstance().GetModels<FileSystemEntryModel>();
      !models.empty()) {
    auto model = models[0];
    ui_->enabled_check_box->setChecked(model->GetIsEnabled());
  }

  // Fills the QTextEdit control with ignored directory names.
  for (const auto& name : Settings::GetInstance().GetIgnoredDirectoryNames()) {
    ui_->ignored_directories_text_edit->append(QString::fromStdString(name));
  }

  connect(ui_->enabled_check_box, &QCheckBox::checkStateChanged, this,
          &FileSearchTab::ToggleFileSystemEntryModel);
  connect(ui_->ignored_directories_text_edit, &QTextEdit::textChanged, this,
          &FileSearchTab::UpdateIgnoredDirectoryNames);
}

FileSearchTab::~FileSearchTab() {}

void FileSearchTab::ToggleFileSystemEntryModel(Qt::CheckState state) {
  auto models = Indexer::GetInstance().GetModels<FileSystemEntryModel>();
  if (models.empty()) {
    return;
  }

  auto model = models[0];
  switch (state) {
    case Qt::Unchecked:
      emit ModelDisabled(model);
      break;
    case Qt::Checked:
      emit ModelEnabled(model);
      break;
    default:
      break;
  }
}

void FileSearchTab::UpdateIgnoredDirectoryNames() {
  auto& settings = Settings::GetInstance();
  settings.ClearIgnoredDirectoryNames();

  auto text = ui_->ignored_directories_text_edit->toPlainText();
  auto parts = text.split('\n');
  for (const auto& part : parts) {
    if (auto name = part.trimmed().toStdString(); !name.empty()) {
      settings.AddIgnoredDirectoryName(name);
    }
  }

  settings.Save();
}
