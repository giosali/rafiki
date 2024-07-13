#include "websearchdialog.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGuiApplication>
#include <QPushButton>

#include "../core/io.h"
#include "./ui_websearchdialog.h"

WebSearchDialog::WebSearchDialog(QWidget* parent)
    : QDialog{parent},
      close_on_show_{false},
      icon_{Io::GetFilePath(Io::Image::kUrl)},
      id_{},
      ui_{std::make_unique<Ui::WebSearchDialog>()} {
  ui_->setupUi(this);
  ui_->interactiveIconLabel->setAlignment(Qt::AlignCenter);
  ui_->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);

  connect(ui_->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(ui_->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(this, &QDialog::accepted, this, &WebSearchDialog::AcceptWebSearch);
  connect(ui_->commandLineEdit, &QLineEdit::textEdited, this,
          &WebSearchDialog::CleanCommandField);
  for (const auto& line_edit : MainLineEdits()) {
    connect(line_edit, &QLineEdit::textEdited, this,
            &WebSearchDialog::CheckMainFields);
  }

  for (const auto& line_edit : AltLineEdits()) {
    connect(line_edit, &QLineEdit::textEdited, this,
            &WebSearchDialog::CheckAltFields);
  }

  connect(ui_->interactiveIconLabel, &InteractiveLabel::Clicked, this,
          &WebSearchDialog::OpenFile);
  connect(ui_->interactiveIconLabel, &InteractiveLabel::MouseEntered,
          []() { QGuiApplication::setOverrideCursor(Qt::PointingHandCursor); });
  connect(ui_->interactiveIconLabel, &InteractiveLabel::MouseLeft,
          &QGuiApplication::restoreOverrideCursor);
}

WebSearchDialog::WebSearchDialog(const Id& id, QWidget* parent)
    : WebSearchDialog{parent} {
  auto web_search = std::dynamic_pointer_cast<WebSearch>(Io::FindResult(id));
  if (web_search == nullptr) {
    // Returns early if the Result isn't a WebSearch or wasn't found.
    close_on_show_ = true;
    return;
  }

  // In-constructor member initialization; this is important.
  icon_ = web_search->GetIcon();
  id_ = id;

  // Fills in fields before showing the dialog to the user.
  ui_->urlLineEdit->setText(web_search->GetUrl());
  ui_->titleLineEdit->setText(web_search->GetTitle());
  ui_->placeholderLabel->setText(web_search->GetTitlePlaceholder());
  ui_->commandLineEdit->setText(web_search->GetCommand());
  ui_->interactiveIconLabel->setPixmap(icon_);
  ui_->altUrlLineEdit->setText(web_search->GetAltUrl());
  ui_->altTitleLineEdit->setText(web_search->GetAltTitle());
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

  if (id_.IsNull()) {
    // Means a WebSearch object is being created.

    Io::AddWebSearch(std::make_shared<WebSearch>(
      url, title, title_placeholder, command, icon_, alt_url, alt_title));
  } else {
    // Means a WebSearch object is being edited.

    auto web_search = std::dynamic_pointer_cast<WebSearch>(Io::FindResult(id_));
    if (web_search == nullptr) {
      // Returns early if the Result isn't a WebSearch or wasn't found.
      return;
    }

    web_search->SetUrl(url);
    web_search->SetTitle(title);
    web_search->SetTitlePlaceholder(title_placeholder);
    web_search->SetIcon(icon_);
    web_search->SetAltUrl(alt_url);
    web_search->SetAltTitle(alt_title);
    Io::EditWebSearch(web_search, command);
  }
}

void WebSearchDialog::CheckAltFields(const QString& text) {
  bool one_empty = false;
  bool one_filled = false;
  for (const auto& line_edit : AltLineEdits()) {
    if (line_edit->text().isEmpty()) {
      one_empty = true;
    } else {
      one_filled = true;
    }
  }

  ToggleSaveButton(one_empty && one_filled ? false : true);
}

void WebSearchDialog::CheckMainFields(const QString& text) {
  for (const auto& line_edit : MainLineEdits()) {
    if (line_edit->text().isEmpty()) {
      ToggleSaveButton(false);
      return;
    }
  }

  // Ensures commands don't contain spaces.
  if (auto command = ui_->commandLineEdit->text(); command.contains(' ')) {
    ToggleSaveButton(false);
    return;
  }

  // This will only be reached if none of the line edits are empty.
  ToggleSaveButton(true);
}

void WebSearchDialog::CleanCommandField(const QString& text) {
  ui_->commandLineEdit->setText(text.simplified().replace(" ", "").toLower());
}

void WebSearchDialog::OpenFile() {
  auto filename = QFileDialog::getOpenFileName(this, "Open Image", QString{},
                                               "Image (*.png *.jpg *.svg)");
  if (filename.isEmpty()) {
    return;
  }

  ui_->interactiveIconLabel->setPixmap(filename);
  icon_ = filename;
}

void WebSearchDialog::showEvent(QShowEvent* event) {
  QDialog::showEvent(event);

  if (close_on_show_) {
    close();
  }
}

std::vector<QLineEdit*> WebSearchDialog::AltLineEdits() const {
  return std::vector<QLineEdit*>{ui_->altUrlLineEdit, ui_->altTitleLineEdit};
}

std::vector<QLineEdit*> WebSearchDialog::MainLineEdits() const {
  return std::vector<QLineEdit*>{ui_->urlLineEdit, ui_->titleLineEdit,
                                 ui_->commandLineEdit};
}

void WebSearchDialog::ToggleSaveButton(bool enable) const {
  ui_->buttonBox->button(QDialogButtonBox::Save)->setEnabled(enable);
}
