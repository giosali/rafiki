#include "mainwindow.h"

#include <QAction>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QIcon>
#include <QMenu>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <Qt>
#include <QtGlobal>

#include "../core/io.h"
#include "./ui_mainwindow.h"
#include "searchbox.h"
#include "searchresultlist.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent},
      settings_window_{std::make_unique<SettingsWindow>(this)},
      ui_{std::make_unique<Ui::MainWindow>()} {
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
  connect(list, &SearchResultList::ItemsChanged, this, &MainWindow::SetHeight);
  connect(box, &SearchBox::TextChanged, list, &SearchResultList::ProcessInput);
  connect(box, &SearchBox::KeyPressed, list,
          &SearchResultList::ProcessKeyPress);
  connect(box, &SearchBox::KeyReleased, list,
          &SearchResultList::ProcessKeyRelease);

  centralWidget()->layout()->addWidget(box);
  centralWidget()->layout()->addWidget(list);

  if (QSystemTrayIcon::isSystemTrayAvailable()) {
    CreateTrayIcon();
  }
}

MainWindow::~MainWindow() {}

void MainWindow::Hide() {
  hide();
  emit Deactivated();
}

void MainWindow::ProcessActivationReason(
  QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::ActivationReason::MiddleClick:
      setVisible(!isVisible());
      break;
  }
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
    QCoreApplication::quit();
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
      if (!isHidden()) {
        Hide();
      }

      break;
  }

  return QMainWindow::event(event);
}

void MainWindow::CreateTrayIcon() {
  auto settings_action = new QAction{"Settings", this};
  connect(settings_action, &QAction::triggered, [this]() {
    Hide();
    settings_window_->show();
  });

  auto quit_action = new QAction{"Quit", this};
  connect(quit_action, &QAction::triggered, this, &QCoreApplication::quit);

  auto tray_menu = new QMenu{this};
  tray_menu->addAction(settings_action);
  tray_menu->addAction(quit_action);

  auto tray_icon = new QSystemTrayIcon{this};
  connect(tray_icon, &QSystemTrayIcon::activated, this,
          &MainWindow::ProcessActivationReason);
  tray_icon->setContextMenu(tray_menu);

  // TODO: change icon.
  tray_icon->setIcon(QIcon{Io::GetFilePath(Io::ImageFile::kUrl)});
  tray_icon->show();
}
