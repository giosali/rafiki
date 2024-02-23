#include "mainwindow.h"

#include <QApplication>
#include <QEvent>
#include <QObject>
#include <QScreen>
#include <QtGlobal>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // Centers the window. On Linux, this will only work on X11 or XWayland.
  move(screen()->geometry().center() - frameGeometry().center());

  // Removes space between Central Widget and the window.
  // -> FMI: https://stackoverflow.com/a/24240025/18831815
  centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
  statusBar()->hide();

  setWindowFlag(Qt::WindowStaysOnTopHint);

  // Hides the QListViewsince it will be empty on initialization.
  findChild<QListView*>("listView")->hide();

  // Sets the height of MainWindow to the height of MainWindow's layout.
  // Leaves the width unchanged by setting it to its own width.
  QRect layoutGeometry = this->centralWidget()->layout()->contentsRect();
  resize(geometry().width(), layoutGeometry.height());
}

MainWindow::~MainWindow() { delete ui; }

QMainWindow* MainWindow::FindMainWindow() {
  for (const auto& widget : QApplication::topLevelWidgets()) {
    QMainWindow* main_window = qobject_cast<QMainWindow*>(widget);
    if (main_window != nullptr) {
      return main_window;
    }
  }

  return nullptr;
}

bool MainWindow::event(QEvent* event) {
  switch (event->type()) {
    case QEvent::WindowDeactivate:
      qDebug() << "Lost focus";
      hide();
      break;
  }

  return QMainWindow::event(event);
}
