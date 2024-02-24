#ifndef MAINLISTWIDGET_H
#define MAINLISTWIDGET_H

#include <QListWidget>

class MainListWidget : public QListWidget {
  Q_OBJECT

 public:
  MainListWidget(QWidget *parent = nullptr);

  void AddItem(const QString& text);
};

#endif  // MAINLISTWIDGET_H
