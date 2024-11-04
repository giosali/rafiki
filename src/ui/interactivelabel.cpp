#include "interactivelabel.h"

InteractiveLabel::InteractiveLabel(QWidget* parent) : QLabel{parent} {}

InteractiveLabel::~InteractiveLabel() {}

void InteractiveLabel::enterEvent(QEnterEvent* event) {
  emit MouseEntered();

  QLabel::enterEvent(event);
}

void InteractiveLabel::leaveEvent(QEvent* event) {
  emit MouseLeft();

  QLabel::leaveEvent(event);
}

void InteractiveLabel::mousePressEvent(QMouseEvent* event) {
  switch (event->button()) {
    case Qt::LeftButton:
      emit Clicked();
      break;
    default:
      break;
  }

  QLabel::mousePressEvent(event);
}
