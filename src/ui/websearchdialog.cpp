#include "websearchdialog.h"

#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QImage>
#include <QJsonObject>
#include <QPushButton>
#include <algorithm>
#include <iterator>

#include "../core/indexer.h"
#include "../core/models/websearchmodel.h"
#include "../core/paths.h"
#include "./ui_websearchdialog.h"

WebSearchDialog::WebSearchDialog(uint64_t id, QWidget* parent)
    : QDialog{parent},
      id_{id},
      is_new_{id == 0},
      ui_{std::make_unique<Ui::WebSearchDialog>()} {
  ui_->setupUi(this);
  ui_->icon_label->setAlignment(Qt::AlignCenter);

  // A new WebSearch object is being created.
  if (is_new_) {
    ui_->button_box->button(QDialogButtonBox::Save)->setEnabled(false);

    // Finds the next available ID that can be used for the new WebSearch
    // object.
    auto models = Indexer::GetInstance().GetModels<WebSearchModel>();
    auto custom_models = std::vector<WebSearchModel*>{};
    std::ranges::copy_if(models, std::back_inserter(custom_models),
                         &WebSearchModel::GetIsCustom);
    auto max =
      std::ranges::max_element(custom_models, {}, &WebSearchModel::GetId);
    id_ = max == custom_models.end() ? 0x8000
                                     : (*max)->GetId() + 1;  // 0x8000 = WM_APP
  }
  // A WebSearch object is being edited.
  else if (auto model =
             dynamic_cast<WebSearchModel*>(Indexer::GetInstance().GetModel(id));
           model != nullptr) {
    ui_->button_box->button(QDialogButtonBox::Save)->setEnabled(true);

    // Fills in fields before showing the dialog to the user.
    ui_->url->setText(model->GetUrl());
    ui_->title->setText(model->GetTitle());
    ui_->placeholder->setCurrentText(model->GetTitlePlaceholder());
    ui_->command->setText(model->GetCommand());
    ui_->icon_label->setPixmap(model->GetIcon());
    ui_->alt_url->setText(model->GetAltUrl());
    ui_->alt_title->setText(model->GetAltTitle());

    icon_path_ = model->GetIconPath();
  }
  // Something went wrong so the dialog should be forcequit.
  else {
    close();
    return;
  }

  connect(ui_->button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(ui_->button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(this, &QDialog::accepted, this, &WebSearchDialog::AcceptWebSearch);
  connect(ui_->url, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->title, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->command, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->alt_url, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->alt_title, &QLineEdit::textChanged, this,
          &WebSearchDialog::CheckFields);
  connect(ui_->icon_label, &InteractiveLabel::Clicked, this,
          &WebSearchDialog::OpenFile);
  connect(ui_->icon_label, &InteractiveLabel::MouseEntered,
          []() { QGuiApplication::setOverrideCursor(Qt::PointingHandCursor); });
  connect(ui_->icon_label, &InteractiveLabel::MouseLeft,
          &QGuiApplication::restoreOverrideCursor);
}

WebSearchDialog::~WebSearchDialog() {}

const QString WebSearchDialog::kFallbackIcon{":/icons/url.svg"};

bool WebSearchDialog::AreAltFieldsValid(const QString& alt_url,
                                        const QString& alt_title) const {
  // Both alt fields must filled if one if them is filled.
  // In other words, if  Alt URL has text, then Alt Title must contain text as
  // well (and vice versa).
  // In more words, they either need to be both empty or both filled.
  auto alt_url_empty = ui_->alt_url->text().isEmpty();
  auto alt_title_empty = ui_->alt_title->text().isEmpty();
  return (alt_url_empty && alt_title_empty) ||
         (!alt_url_empty && !alt_title_empty);
}

bool WebSearchDialog::AreRequiredFieldsValid(const QString& url,
                                             const QString& title,
                                             const QString& title_placeholder,
                                             const QString& command) const {
  // None of the required fields can be empty.
  return !url.isEmpty() && !title.isEmpty() && !title_placeholder.isEmpty() &&
         !command.isEmpty();
}

QString WebSearchDialog::SaveIcon(const QString& path) const {
  auto original = QImage{path};
  if (original.isNull() || path == kFallbackIcon) {
    return kFallbackIcon;
  }

  auto icon_dir = QDir{Paths::GetPath(Paths::Directory::kUserIcons)};
  auto icon_name = QString{"%1.%2"}.arg(id_).arg(QFileInfo{path}.suffix());
  auto new_path = icon_dir.filePath(icon_name);

  // Creates all necessary parent directories.
  QDir{}.mkpath(icon_dir.path());

  // Resizes new image to a width of 128 only if it's greater than 128.
  if (original.width() > 128) {
    auto resized = original.scaledToWidth(128, Qt::SmoothTransformation);
    if (!resized.save(new_path)) {
      return kFallbackIcon;
    }
  } else {
    if (!original.save(new_path)) {
      return kFallbackIcon;
    }
  }

  return new_path;
}

void WebSearchDialog::AcceptWebSearch() {
  auto url = ui_->url->text();
  auto title = ui_->title->text();
  auto title_placeholder = ui_->placeholder->currentText();
  auto command = ui_->command->text();
  auto alt_url = ui_->alt_url->text();
  auto alt_title = ui_->alt_title->text();
  if (!AreRequiredFieldsValid(url, title, title_placeholder, command) ||
      !AreAltFieldsValid(alt_url, alt_title)) {
    return;
  }

  icon_path_ = SaveIcon(icon_path_);

  // A WebSearchModel instance is being created.
  if (auto& indexer = Indexer::GetInstance(); is_new_) {
    auto object = QJsonObject{{"command", command},
                              {"icon", icon_path_},
                              {"id", QString::number(id_)},
                              {"title", title},
                              {"placeholder", title_placeholder},
                              {"url", url},
                              {"isCustom", true},
                              {
                                "alt",
                                QJsonObject{
                                  {"title", alt_title},
                                  {"url", alt_url},
                                },
                              }};
    indexer.IndexModel(std::make_unique<WebSearchModel>(object));
  }
  // A WebSearchModel instance is being edited.
  else if (auto model = dynamic_cast<WebSearchModel*>(indexer.GetModel(id_));
           model != nullptr) {
    auto old_tokens = model->Tokenize();

    // Moves previous icon to trash. It's okay for fileName to be null or empty.
    auto old_icon_path = model->GetIconPath();
    QFile::moveToTrash(old_icon_path);

    model->SetCommand(command);
    model->SetIcon(icon_path_);
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
  auto command = ui_->command->text();
  if (command.contains(' ') || !command.isLower()) {
    auto clean_command = command.simplified().replace(" ", "").toLower();
    ui_->command->setText(clean_command);
    return;
  }

  // Disables save button if any of the main fields are empty
  // or if only one of the alt fields is filled.
  ui_->button_box->button(QDialogButtonBox::Save)
    ->setEnabled(
      AreRequiredFieldsValid(ui_->url->text(), ui_->title->text(),
                             ui_->placeholder->currentText(), command) &&
      AreAltFieldsValid(ui_->alt_url->text(), ui_->alt_title->text()));
}

void WebSearchDialog::OpenFile() {
  if (auto filename = QFileDialog::getOpenFileName(
        this, "Open Image", QString{}, "Image (*.png *.jpg)");
      !filename.isEmpty()) {
    icon_path_ = filename;
    ui_->icon_label->setPixmap(filename);
  }
}
