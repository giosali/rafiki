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

void SearchBox::keyPressEvent(QKeyEvent* event) {
  auto key = event->key();
  switch (key) {
    case Qt::Key::Key_Return:
    case Qt::Key::Key_Up:
    case Qt::Key::Key_Down:
      emit KeyPressed(key);
      break;
  }
}
