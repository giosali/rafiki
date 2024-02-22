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

  // Centers the window.
  move(screen()->geometry().center() - frameGeometry().center());

  // Removes space between Central Widget and the window.
  // -> FMI: https://stackoverflow.com/a/24240025/18831815
  centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
  statusBar()->hide();

  setWindowFlag(Qt::WindowStaysOnTopHint);
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
