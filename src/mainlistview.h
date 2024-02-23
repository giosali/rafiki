#ifndef MAINLISTVIEW_H
#define MAINLISTVIEW_H

#include <QListView>

class MainListView : public QListView {
  Q_OBJECT

 public:
  MainListView(QWidget *parent = nullptr);
};

#endif  // MAINLISTVIEW_H
