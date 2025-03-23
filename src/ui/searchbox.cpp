#include "searchbox.h"

#include <QFontDatabase>
#include <QStyle>
#include <Qt>

#include "../core/models/filesystementrymodel.h"
#include "./ui_searchbox.h"

SearchBox::SearchBox(QWidget* parent)
    : QWidget{parent}, ui_{std::make_unique<Ui::SearchBox>()} {
  ui_->setupUi(this);

  connect(ui_->line_edit, &QLineEdit::textChanged, this,
          &SearchBox::ProcessText);
  connect(ui_->line_edit, &CustomLineEdit::KeyPressed,
          [this](const QKeyCombination& combination) {
            emit KeyPressed(combination);
          });
  connect(ui_->line_edit, &CustomLineEdit::KeyReleased,
          [this](const QKeyCombination& combination) {
            emit KeyReleased(combination);
          });
}

SearchBox::~SearchBox() {}

QString SearchBox::GetText() const { return ui_->line_edit->text(); }

int SearchBox::Height() const { return ui_->layout->sizeHint().height(); }

void SearchBox::ApplyTheme(Theme* theme) {
  auto stylesheet = QString{R"(
    QLineEdit {
      background-color: %1;
      border: none;
      border-bottom-left-radius: 0;
      border-bottom-right-radius: 0;
      border-top-left-radius: %2px;
      border-top-right-radius: %3px;
      color: %4;
      font-size: %5px;
      font-weight: 500;
      padding: 12px 8px;
    }

    QLineEdit[text=""] {
      border-radius: %6px;
    }
  )"}
                      .arg(theme->GetWindowBackgroundColor().name())
                      .arg(theme->GetBorderRadius())
                      .arg(theme->GetBorderRadius())
                      .arg(theme->GetViewForegroundColor().name())
                      .arg(theme->GetFontSize())
                      .arg(theme->GetBorderRadius());
  ui_->line_edit->setStyleSheet(stylesheet);

  // It might be safe to remove this.
  setFixedHeight(Height());
}

void SearchBox::Clear() { ui_->line_edit->clear(); }

void SearchBox::SetText(const QString& text) { ui_->line_edit->setText(text); }

void SearchBox::ProcessText(const QString& text) {
  // This should be initialized to true since the QLineEdit will be empty when
  // it appears for the first time.
  static bool is_box_empty = true;

  // Changes the theming of the SearchBox based on whether or not the
  // SearchBox's internal QLineEdit contains text.
  // This helps to stave off excessive and unnecessary `unpolish` and `polish`
  // calls.
  if (text.isEmpty() && !is_box_empty) {
    is_box_empty = true;
    auto style = ui_->line_edit->style();
    style->unpolish(ui_->line_edit);
    style->polish(ui_->line_edit);
  } else if (!text.isEmpty() && is_box_empty) {
    is_box_empty = false;
    auto style = ui_->line_edit->style();
    style->unpolish(ui_->line_edit);
    style->polish(ui_->line_edit);
  }

  // This block is exclusively for the FileSystemEntryObject.
  if (text == " ") {
    ui_->line_edit->setText(FileSystemEntryModel::kCommand + " ");
  } else {
    emit TextChanged(text);
  }
}
