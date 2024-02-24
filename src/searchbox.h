#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QLineEdit>

class SearchBox : public QLineEdit {
  Q_OBJECT

 public:
  SearchBox(QWidget* parent = nullptr);

 public slots:
  void TextChanged(const QString& text);
};

#endif  // SEARCHBOX_H
