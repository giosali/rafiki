#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QMainWindow>
#include <QString>
#include <memory>

#include "../core/input.h"
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

  static MainWindow *Get();
  Input GetSearchBoxText();
  void SetSearchBoxText(const QString &text);
  void SetSearchResultDescription(const QString &text);
  void SetSearchResultTitle(const QString &text);

 public slots:
  void Hide();
  void ProcessCommandLineArguments(const QString &args);
  void SetHeight(SearchResultList *list);

 signals:
  void Deactivated();

 protected:
  bool event(QEvent *event) override;

 private:
  std::unique_ptr<Ui::MainWindow> ui_;
};
#endif  // MAINWINDOW_H
