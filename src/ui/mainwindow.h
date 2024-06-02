#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QMainWindow>
#include <QString>
#include <memory>

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

 public slots:
  void Hide();
  void ProcessCommandLineArguments(const QString &args);
  void SetHeight(int height);

 signals:
  void Deactivated();

 protected:
  bool event(QEvent *event) override;

 private:
  std::unique_ptr<Ui::MainWindow> ui_;
};
#endif  // MAINWINDOW_H
