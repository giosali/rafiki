#include "websearchdialog.h"

#include <QDialogButtonBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QJsonObject>
#include <QPushButton>
#include <memory>

#include "../core/file.h"
#include "../core/indexer.h"
#include "../core/models/websearchmodel.h"
#include "../core/paths.h"
#include "../core/settings.h"
#include "../core/utilities.h"
#include "./ui_websearchdialog.h"

WebSearchDialog::WebSearchDialog(QWidget* parent)
    : QDialog{parent}, ui_{std::make_unique<Ui::WebSearchDialog>()} {
  ui_->setupUi(this);
  ui_->interactiveIconLabel->setAlignment(Qt::AlignCenter);
  ui_->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
  ToggleSaveButton(false);  // Disabled when adding a WebSearch.

  connect(ui_->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(ui_->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(this, &QDialog::accepted, this, &WebSearchDialog::AcceptWebSearch);
  connect(ui_->urlLineEdit, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->titleLineEdit, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->commandLineEdit, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->altUrlLineEdit, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->altTitleLineEdit, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->interactiveIconLabel, &InteractiveLabel::Clicked, this,
          &WebSearchDialog::OpenFile);
  connect(ui_->interactiveIconLabel, &InteractiveLabel::MouseEntered,
          []() { QGuiApplication::setOverrideCursor(Qt::PointingHandCursor); });
  connect(ui_->interactiveIconLabel, &InteractiveLabel::MouseLeft,
          &QGuiApplication::restoreOverrideCursor);
}

WebSearchDialog::WebSearchDialog(uint64_t id, QWidget* parent)
    : WebSearchDialog{parent} {
  ToggleSaveButton(true);  // Enabled when editing a WebSearch.

  auto model =
    dynamic_cast<WebSearchModel*>(Indexer::GetInstance().GetModel(id));
  if (model == nullptr) {
    close();
    return;
  }

  // In-constructor member initialization; this is important.
  current_icon_path_ = model->GetIconPath();
  id_ = id;

  // Fills in fields before showing the dialog to the user.
  ui_->urlLineEdit->setText(model->GetUrl());
  ui_->titleLineEdit->setText(model->GetTitle());
  ui_->placeholderComboBox->setCurrentText(model->GetTitlePlaceholder());
  // ui_->placeholderLabel->setText(model->GetTitlePlaceholder());
  ui_->commandLineEdit->setText(model->GetCommand());
  ui_->interactiveIconLabel->setPixmap(model->GetIcon());
  ui_->altUrlLineEdit->setText(model->GetAltUrl());
  ui_->altTitleLineEdit->setText(model->GetAltTitle());
}

WebSearchDialog::~WebSearchDialog() {}

void WebSearchDialog::AcceptWebSearch() {
  auto url = ui_->urlLineEdit->text();
  auto title = ui_->titleLineEdit->text();
  auto title_placeholder = ui_->placeholderComboBox->currentText();
  auto command = ui_->commandLineEdit->text();

  if (url.isEmpty() || title.isEmpty() || title_placeholder.isEmpty() ||
      command.isEmpty()) {
    return;
  }

  auto alt_url = ui_->altUrlLineEdit->text();
  auto alt_title = ui_->altTitleLineEdit->text();

  // Fails if one is empty and the other not.
  if ((alt_url.isEmpty() && !alt_title.isEmpty()) ||
      (!alt_url.isEmpty() && alt_title.isEmpty())) {
    return;
  }

  auto is_being_created = id_ == 0;
  auto icon_path = QString{};
  if (new_icon_path_.isNull()) {
    icon_path = is_being_created ? Paths::GetPath(Paths::Image::kUrl)
                                 : current_icon_path_;
  } else {
    // Moves previosu icon to trash.
    QFile::moveToTrash(current_icon_path_);

    auto icon_dir = Paths::GetPath(Paths::Directory::kUserIcons);
    auto icon_name =
      QString{"%1.%2"}
        .arg(is_being_created ? Settings::GetInstance().GetAvailableId() : id_)
        .arg(QFileInfo{new_icon_path_}.suffix());
    icon_path = Utilities::Combine(icon_dir, icon_name);

    // Saves new icon to config directory.
    File::Copy(new_icon_path_, icon_path);
  }

  auto& indexer = Indexer::GetInstance();
  if (is_being_created) {
    auto& settings = Settings::GetInstance();
    auto available_id = settings.GetAvailableId();

    auto object =
      QJsonObject{{"command", command},
                  {"icon", icon_path},
                  {"id", QString::number(available_id)},
                  {"title", title},
                  {"placeholder", title_placeholder},
                  {"url", url},
                  {"isCustom", true},
                  {"alt", QJsonObject{{"title", alt_title}, {"url", alt_url}}}};
    indexer.IndexModel(std::make_unique<WebSearchModel>(object));

    // Increments and saves used ID.
    settings.SetAvailableId(available_id + 1);
    settings.Save();
  } else {
    // A WebSearchModel instance is being edited.
    auto model = dynamic_cast<WebSearchModel*>(indexer.GetModel(id_));
    auto old_tokens = model->Tokenize();

    model->SetCommand(command);
    model->SetIcon(icon_path);
    model->SetTitle(title);
    model->SetTitlePlaceholder(title_placeholder);
    model->SetUrl(url);
    model->SetAltTitle(alt_title);
    model->SetAltUrl(alt_url);

    auto new_tokens = model->Tokenize();

    indexer.UpdateTrie(model->GetId(), old_tokens, new_tokens);
  }
}

void WebSearchDialog::CheckFields(const QString& text) {
  // Returns early if the command contains a space or is not lowercase, "cleans"
  // the command, and then sets the command edit widget to the clean command. We
  // return early because programmatically setting the text will trigger this
  // slot function again.
  auto command = ui_->commandLineEdit->text();
  if (command.contains(' ') || !command.isLower()) {
    auto clean_command = command.simplified().replace(" ", "").toLower();
    ui_->commandLineEdit->setText(clean_command);
    return;
  }

  // Disables save button if any of the main fields are empty.
  auto url = ui_->urlLineEdit->text();
  auto title = ui_->titleLineEdit->text();
  if (url.isEmpty() || title.isEmpty() || command.isEmpty()) {
    ToggleSaveButton(false);
    return;
  }

  // Disables save button if only one of the alt fields is filled.
  auto alt_url_empty = ui_->altUrlLineEdit->text().isEmpty();
  auto alt_title_empty = ui_->altTitleLineEdit->text().isEmpty();
  if ((alt_url_empty && !alt_title_empty) ||
      (!alt_url_empty && alt_title_empty)) {
    ToggleSaveButton(false);
    return;
  }

  ToggleSaveButton(true);
}

void WebSearchDialog::CleanCommandField(const QString& text) {
  ui_->commandLineEdit->setText(text.simplified().replace(" ", "").toLower());
}

void WebSearchDialog::OpenFile() {
  if (auto filename = QFileDialog::getOpenFileName(
        this, "Open Image", QString{}, "Image (*.png *.jpg *.svg)");
      !filename.isEmpty()) {
    new_icon_path_ = filename;
    ui_->interactiveIconLabel->setPixmap(filename);
  }
}

void WebSearchDialog::ToggleSaveButton(bool enable) const {
  ui_->buttonBox->button(QDialogButtonBox::Save)->setEnabled(enable);
}
