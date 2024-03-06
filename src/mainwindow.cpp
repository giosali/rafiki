#include "mainwindow.h"

#include <QApplication>
#include <QEvent>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <Qt>
#include <QtGlobal>
#include <utility>

#include "./ui_mainwindow.h"
#include "searchbox.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui_(std::make_unique<Ui::MainWindow>()) {
  ui_->setupUi(this);
  setWindowFlag(Qt::WindowStaysOnTopHint);

  // Prevents child widgets from changing the width of the window.
  setFixedWidth(width());

  // Sets opening window size to just the SearchBox.
  resize(width(), 0);

  // Horizontally centers and properly vertically aligns the window. On Linux,
  // this will only work on X11 or XWayland.
  auto screen_geometry = screen()->geometry();
  auto p = screen_geometry.center() - frameGeometry().center();
  p.setY(static_cast<int>(screen_geometry.height() * 0.175));
  move(p);

  auto search_box = std::make_unique<SearchBox>(this);
  auto list = std::make_unique<SearchResultList>(this);

  // Prevents the window height from strangely expanding when input is cleared.
  setMinimumHeight(search_box->Height());

  QObject::connect(list.get(), &SearchResultList::ItemsAdded, this,
                   &MainWindow::SetHeight);
  QObject::connect(list.get(), &SearchResultList::ItemsCleared, this,
                   &MainWindow::SetHeight);
  QObject::connect(search_box.get(), &SearchBox::TextChanged, list.get(),
                   &SearchResultList::ProcessInput);
  QObject::connect(search_box.get(), &SearchBox::KeyPressed, list.get(),
                   &SearchResultList::ProcessKeyPress);

  centralWidget()->layout()->addWidget(search_box.release());
  centralWidget()->layout()->addWidget(list.release());
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
    // Setting the minimum height prevents the window from shrinking to 0, which
    // is a good thing.
    resize(width(), 0);
    return;
  }

  resize(width(), minimumHeight() + list_height);
}

bool MainWindow::event(QEvent* event) {
  switch (event->type()) {
    case QEvent::WindowDeactivate:
      hide();
      break;
  }

  return QMainWindow::event(event);
}
