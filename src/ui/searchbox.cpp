#include "searchbox.h"

#include <Qt>

#include "../core/models/filesystementrymodel.h"
#include "./ui_searchbox.h"

SearchBox::SearchBox(QWidget* parent)
    : QWidget{parent}, ui_{std::make_unique<Ui::SearchBox>()} {
  ui_->setupUi(this);

  connect(ui_->searchBox, &QLineEdit::textChanged, [this](const QString& text) {
    // This block is exclusively for the FileSystemEntryObject.
    if (text == " ") {
      ui_->searchBox->setText(FileSystemEntryModel::kCommand + " ");
      return;
    }

    emit TextChanged(text);
  });
}

SearchBox::~SearchBox() {}

int SearchBox::Height() const { return ui_->layout->sizeHint().height(); }

QString SearchBox::GetText() const { return ui_->searchBox->text(); }

void SearchBox::ApplyTheme(Theme* theme) {
  auto stylesheet =
    QString{"QLineEdit { border: none; font-size: %1px; padding: 12px 8px; }"}
      .arg(theme->GetFontSize());
  ui_->searchBox->setStyleSheet(stylesheet);

  // It might be safe to remove this.
  setFixedHeight(Height());
}

void SearchBox::Clear() { ui_->searchBox->clear(); }

void SearchBox::SetText(const QString& text) { ui_->searchBox->setText(text); }

void SearchBox::keyPressEvent(QKeyEvent* event) {
  auto combination = event->keyCombination();
  switch (combination.key()) {
    case Qt::Key_Tab:
    case Qt::Key_Return:
    case Qt::Key_Alt:
      if (event->isAutoRepeat()) {
        return;
      }

      [[fallthrough]];
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
      emit KeyPressed(combination);
      break;
    default:
      break;
  }
}

void SearchBox::keyReleaseEvent(QKeyEvent* event) {
  auto combination = event->keyCombination();
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit KeyReleased(combination);
      break;
    default:
      break;
  }
}
