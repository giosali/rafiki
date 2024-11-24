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

#include "../core/paths.h"
#include "./ui_mainwindow.h"
#include "searchbox.h"
#include "searchresultlist.h"
#include "settingswindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{std::make_unique<Ui::MainWindow>()} {
  ui_->setupUi(this);

  // Prevents child widgets from changing the width of the window.
  setFixedWidth(width());

  auto box = new SearchBox(this);
  auto list = new SearchResultList(box, this);

  // Prevents the window height from strangely expanding when input is cleared.
  setMinimumHeight(box->Height());

  connect(this, &MainWindow::Deactivated, box, &SearchBox::Clear);
  connect(list, &SearchResultList::ItemsChanged, this, &MainWindow::SetHeight);
  connect(box, &SearchBox::TextChanged, list, &SearchResultList::ProcessText);
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

void MainWindow::OpenSettingsWindow() {
  Hide();
  (new SettingsWindow{this})->show();
}

void MainWindow::ProcessActivationReason(
  QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::ActivationReason::MiddleClick:
      ToggleVisibility();
      break;
    default:
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

  if (!parser.parse(args.split(" "))) {
    return;
  }

  if (parser.isSet(toggleOption)) {
    ToggleVisibility();
  }

  if (parser.isSet(quitOption)) {
    QCoreApplication::quit();
  }
}

void MainWindow::SetHeight(int height) {
  // The minimum height of the window being already set prevents the window from
  // shrinking down all the way to 0, which is a good thing.
  resize(width(), minimumHeight() + height);
}

bool MainWindow::event(QEvent* event) {
  switch (event->type()) {
    case QEvent::WindowActivate: {
      // Repositions the window whenever it becomes activated.
      // I opted for `screen()->size()` rather than `screen()->geometry()`
      // because only the `width` and `height` are relevant. The `x` and `y`
      // properties don't matter and seem to always be `0,0`; I'm not sure if
      // they're always `0,0` though.
      // On Linux, this only works on X11 or XWayland.
      auto screen_size = screen()->size() / 2;  // "Centers" the size.
      auto window_size = frameSize() / 2;       // "Centers" the size.
      auto screen_point = QPoint{screen_size.width(), screen_size.height()};
      auto window_point = QPoint{window_size.width(), window_size.height()};
      auto point = screen_point - window_point;
      point.setY(static_cast<int>(screen_size.height() * 0.35));
      move(point);
      break;
    }
    case QEvent::WindowDeactivate:  // Window lost focus.
      if (!isHidden()) {
        Hide();
      }

      break;
    default:
      break;
  }

  return QMainWindow::event(event);
}

void MainWindow::CreateTrayIcon() {
  auto settings_action = new QAction{"Settings", this};
  connect(settings_action, &QAction::triggered, this,
          &MainWindow::OpenSettingsWindow);

  auto quit_action = new QAction{"Quit", this};
  connect(quit_action, &QAction::triggered, this, &QCoreApplication::quit);

  auto tray_menu = new QMenu{this};
  tray_menu->addAction(settings_action);
  tray_menu->addAction(quit_action);

  auto tray_icon = new QSystemTrayIcon{this};
  connect(tray_icon, &QSystemTrayIcon::activated, this,
          &MainWindow::ProcessActivationReason);
  tray_icon->setContextMenu(tray_menu);

  tray_icon->setIcon(QIcon{Paths::GetPath(Paths::Image::kRafiki)});
  tray_icon->show();
}

void MainWindow::ToggleVisibility() {
  if (isHidden()) {
    show();
    activateWindow();
    raise();
  } else {
    Hide();
  }
}
