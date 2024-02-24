#ifndef MAINLISTWIDGET_H
#define MAINLISTWIDGET_H

#include <QListWidget>

class MainListWidget : public QListWidget {
  Q_OBJECT

 public:
  MainListWidget(QWidget *parent = nullptr);
};

#endif  // MAINLISTWIDGET_H
