#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
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
    : QMainWindow{parent}, ui_{std::make_unique<Ui::MainWindow>()} {
  ui_->setupUi(this);
  setWindowFlag(Qt::WindowStaysOnTopHint);

  // TODO: uncomment this when a proper exit method has been developed.
  // setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

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

  QObject::connect(this, &MainWindow::Deactivated, search_box.get(),
                   &SearchBox::Clear);
  QObject::connect(list.get(), &SearchResultList::HideWindowRequested, this,
                   [this]() { hide(); });
  QObject::connect(list.get(), &SearchResultList::ItemsChanged, this,
                   &MainWindow::SetHeight);
  QObject::connect(list.get(), &SearchResultList::TextReceived,
                   search_box.get(), &SearchBox::SetText);
  QObject::connect(list.get(), &SearchResultList::EventReceived,
                   search_box.get(), &SearchBox::SimulateKeyPress);
  QObject::connect(search_box.get(), &SearchBox::TextChanged, list.get(),
                   &SearchResultList::ProcessInput);
  QObject::connect(search_box.get(), &SearchBox::KeyPressed, list.get(),
                   &SearchResultList::ProcessKeyPress);
  QObject::connect(search_box.get(), &SearchBox::KeyReleased, list.get(),
                   &SearchResultList::ProcessKeyRelease);

  centralWidget()->layout()->addWidget(search_box.release());
  centralWidget()->layout()->addWidget(list.release());
}

MainWindow::~MainWindow() {}

void MainWindow::ProcessCommandLineArguments(const QString& args) {
  auto parser = QCommandLineParser{};

  // Sets up builtin options.
  parser.addHelpOption();
  parser.addVersionOption();

  // Sets up custom options.
  QCommandLineOption toggleOption(
    "toggle", "Toggles the visibility of the main input window.");
  parser.addOption(toggleOption);
  QCommandLineOption quitOption("quit", "Quits the application.");
  parser.addOption(quitOption);

  auto arg_list = args.split(" ");
  if (!parser.parse(arg_list)) {
    return;
  }

  if (parser.isSet(toggleOption)) {
    if (isVisible()) {
      hide();
    } else {
      show();
      activateWindow();
    }
  }

  if (parser.isSet(quitOption)) {
    QApplication::quit();
  }
}

void MainWindow::SetHeight(SearchResultList* list) {
  // The minimum height of the window being already set prevents the window from
  // shrinking down all the way to 0, which is a good thing.
  auto list_height = list->Height();
  resize(width(), list_height == 0 ? 0 : minimumHeight() + list_height);
}

bool MainWindow::event(QEvent* event) {
  switch (event->type()) {
    case QEvent::WindowDeactivate:
      hide();
      emit Deactivated();
      break;
  }

  return QMainWindow::event(event);
}
