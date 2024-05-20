#include "searchbox.h"

#include <Qt>

#include "./ui_searchbox.h"

SearchBox::SearchBox(QWidget* parent)
    : QWidget{parent}, ui_{std::make_unique<Ui::SearchBox>()} {
  ui_->setupUi(this);
  setFixedHeight(Height());

  connect(ui_->searchBox, &QLineEdit::textChanged,
          [this](const QString& text) { emit TextChanged(Input{text}); });
}

SearchBox::~SearchBox() {}

int SearchBox::Height() const { return ui_->layout->sizeHint().height(); }

Input SearchBox::GetText() const { return Input{ui_->searchBox->text()}; }

void SearchBox::Clear() { ui_->searchBox->clear(); }

void SearchBox::SetText(const QString& text) { ui_->searchBox->setText(text); }

void SearchBox::keyPressEvent(QKeyEvent* event) {
  if (event->isAutoRepeat()) {
    return;
  }

  auto combination = event->keyCombination();
  switch (combination.key()) {
    case Qt::Key_Tab:
    case Qt::Key_Return:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Alt:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
      emit KeyPressed(combination);
      break;
  }
}

void SearchBox::keyReleaseEvent(QKeyEvent* event) {
  auto combination = event->keyCombination();
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit KeyReleased(combination);
      break;
  }
}
