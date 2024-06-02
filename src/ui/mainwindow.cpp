#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <Qt>
#include <QtGlobal>

#include "./ui_mainwindow.h"
#include "searchbox.h"
#include "searchresultlist.h"

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

  auto box = new SearchBox(this);
  auto list = new SearchResultList(box, this);

  // Prevents the window height from strangely expanding when input is cleared.
  setMinimumHeight(box->Height());

  connect(this, &MainWindow::Deactivated, box, &SearchBox::Clear);
  connect(list, &SearchResultList::HideRequested, this, &MainWindow::Hide);
  connect(list, &SearchResultList::ItemsChanged, this, &MainWindow::SetHeight);
  connect(box, &SearchBox::TextChanged, list, &SearchResultList::ProcessInput);
  connect(box, &SearchBox::KeyPressed, list,
          &SearchResultList::ProcessKeyPress);
  connect(box, &SearchBox::KeyReleased, list,
          &SearchResultList::ProcessKeyRelease);

  centralWidget()->layout()->addWidget(box);
  centralWidget()->layout()->addWidget(list);
}

MainWindow::~MainWindow() {}

void MainWindow::Hide() {
  hide();
  emit Deactivated();
}

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

void MainWindow::SetHeight(int height) {
  // The minimum height of the window being already set prevents the window from
  // shrinking down all the way to 0, which is a good thing.
  resize(width(), height == 0 ? 0 : minimumHeight() + height);
}

bool MainWindow::event(QEvent* event) {
  switch (event->type()) {
    case QEvent::WindowDeactivate:  // Window lost focus.
      if (isHidden()) {
        break;
      }

      Hide();
      break;
  }

  return QMainWindow::event(event);
}
