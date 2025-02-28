#include "filesearchtab.h"

#include <QCheckBox>
#include <QTextEdit>

#include "../core/indexer.h"
#include "../core/models/filesystementrymodel.h"
#include "../core/settings.h"
#include "./ui_filesearchtab.h"

FileSearchTab::FileSearchTab(QWidget* parent)
    : ToggleTab{parent}, ui_{std::make_unique<Ui::FileSearchTab>()} {
  ui_->setupUi(this);

  // Checks or unchecks QCheckBox control.
  if (auto models = Indexer::GetInstance().GetModels<FileSystemEntryModel>();
      !models.empty()) {
    ui_->enabled_check_box->setChecked(models[0]->GetIsEnabled());
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
  if (auto models = Indexer::GetInstance().GetModels<FileSystemEntryModel>();
      !models.empty()) {
    emit ModelToggled(state, models[0]->GetId());
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
