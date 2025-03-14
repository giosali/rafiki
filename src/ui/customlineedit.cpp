#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget* parent) : QLineEdit{parent} {}

CustomLineEdit::~CustomLineEdit() {}

void CustomLineEdit::keyPressEvent(QKeyEvent* event) {
  auto combination = event->keyCombination();
  switch (combination.key()) {
    case Qt::Key_Tab:
    case Qt::Key_Return:
    case Qt::Key_Alt:
      if (event->isAutoRepeat()) {
        return;
      }

      [[fallthrough]];
    case Qt::Key_K:
    case Qt::Key_T:
    case Qt::Key_Escape:
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

  // This is important for text to be able to actually appear in the QLineEdit.
  QLineEdit::keyPressEvent(event);
}

void CustomLineEdit::keyReleaseEvent(QKeyEvent* event) {
  auto combination = event->keyCombination();
  if (combination.key() == Qt::Key_Alt) {
    emit KeyReleased(combination);
  }

  QLineEdit::keyReleaseEvent(event);
}
