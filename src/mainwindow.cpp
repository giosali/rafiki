#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QObject>
#include <QScreen>
#include <QtGlobal>
#include <utility>

#include "./ui_mainwindow.h"
#include "searchbox.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui_(std::make_unique<Ui::MainWindow>()) {
  ui_->setupUi(this);
  setWindowFlag(Qt::WindowStaysOnTopHint);
  centralWidget()->layout()->setSpacing(0);

  // Centers the window. On Linux, this will only work on X11 or XWayland.
  move(screen()->geometry().center() - frameGeometry().center());

  // Removes space between Central Widget and the window.
  // See: https://stackoverflow.com/a/24240025/18831815
  centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
  statusBar()->hide();

  auto search_box = std::make_unique<SearchBox>(this);
  auto list = std::make_unique<SearchResultList>(this);

  QObject::connect(list.get(), &SearchResultList::ItemsAdded, this,
                   &MainWindow::SetHeight);
  QObject::connect(list.get(), &SearchResultList::ItemsCleared, this,
                   &MainWindow::SetHeight);
  QObject::connect(search_box.get(), &SearchBox::TextChanged, list.get(),
                   &SearchResultList::ProcessInput);

  centralWidget()->layout()->addWidget(search_box.release());
  centralWidget()->layout()->addWidget(list.release());

  AdjustSize();
}

MainWindow::~MainWindow() {}

QMainWindow* MainWindow::FindMainWindow() {
  for (const auto& widget : QApplication::topLevelWidgets()) {
    auto main_window = qobject_cast<QMainWindow*>(widget);
    if (main_window == nullptr) {
      continue;
    }

    return main_window;
  }

  return nullptr;
}

void MainWindow::SetHeight(SearchResultList* list) {
  auto list_height = list->Height();
  if (list_height == 0) {
    AdjustSize();
    qDebug() << "list_height: zero";
    return;
  }

  auto window_width = geometry().width();
  auto search_box = findChild<SearchBox*>("SearchBox");
  resize(window_width, list_height + search_box->height());
}

bool MainWindow::event(QEvent* event) {
  switch (event->type()) {
    case QEvent::WindowDeactivate:
      hide();
      break;
  }

  return QMainWindow::event(event);
}

void MainWindow::AdjustSize() {
  // Sets the height of MainWindow to the height of MainWindow's layout.
  // Leaves the width unchanged by essentially setting it to its own width.
  auto window_width = geometry().width();
  auto search_box = findChild<SearchBox*>("SearchBox");
  auto search_box_height = search_box->Height();
  search_box->resize(window_width, search_box_height);
  qDebug() << "search_box inner height = " << search_box_height;
  // auto layout_height = centralWidget()->layout()->contentsRect().height();
  // qDebug() << "LAYOUT_HEIGHT = " << layout_height;
  resize(window_width, 0);
}
