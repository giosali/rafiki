#include "searchbox.h"

#include <QObject>
#include <QtGlobal>

SearchBox::SearchBox(QWidget* parent) : QLineEdit(parent) {
  QObject::connect(this, &QLineEdit::textChanged, this,
                   &SearchBox::TextChanged);
}

void SearchBox::TextChanged(const QString& text) {
  qDebug() << "Text changed: " << text;
}
