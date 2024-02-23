#include "mainlineedit.h"

#include <QObject>
#include <QtGlobal>

MainLineEdit::MainLineEdit(QWidget* parent) : QLineEdit(parent) {
  QObject::connect(this, &QLineEdit::textChanged, this,
                   &MainLineEdit::TextChanged);
}

void MainLineEdit::TextChanged(const QString& text) {
  qDebug() << "Text changed: " << text;
}
