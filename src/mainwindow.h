#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "searchresultlist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

  static QMainWindow *FindMainWindow();

 public slots:
  void SetHeight(SearchResultList *list);

 protected:
  bool event(QEvent *event) override;

 private:
  void AdjustSize();

  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
