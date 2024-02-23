#ifndef MAINLINEEDIT_H
#define MAINLINEEDIT_H

#include <QLineEdit>

class MainLineEdit : public QLineEdit {
  Q_OBJECT

 public:
  MainLineEdit(QWidget* parent = nullptr);
 public slots:
  void TextChanged(const QString& text);
};

#endif  // MAINLINEEDIT_H
