#include "searchbox.h"

#include <QObject>
#include <Qt>

#include "./ui_searchbox.h"

SearchBox::SearchBox(QWidget* parent)
    : QWidget(parent), ui_(std::make_unique<Ui::SearchBox>()) {
  ui_->setupUi(this);
  setFixedHeight(Height());

  QObject::connect(ui_->searchBox, &QLineEdit::textChanged, this,
                   &SearchBox::EmitTextChanged);
}

SearchBox::~SearchBox() {}

int SearchBox::Height() const { return ui_->layout->sizeHint().height(); }

void SearchBox::EmitTextChanged(const QString& text) { emit TextChanged(text); }

void SearchBox::SetText(const QString& text) { ui_->searchBox->setText(text); }

void SearchBox::keyPressEvent(QKeyEvent* event) {
  auto key_combination = event->keyCombination();
  switch (key_combination.key()) {
    case Qt::Key_Tab:
    case Qt::Key_Return:
    case Qt::Key_Up:
    case Qt::Key_Down:
      emit KeyPressed(key_combination);
      break;
  }
}
