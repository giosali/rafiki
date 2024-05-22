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
  auto list = new SearchResultList(this);

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

MainWindow* MainWindow::Get() {
  return qobject_cast<MainWindow*>(QApplication::activeWindow());
}

Input MainWindow::GetSearchBoxText() {
  auto box = findChild<SearchBox*>("SearchBox");
  if (box == nullptr) {
    return Input{};
  }

  return box->GetText();
}

void MainWindow::Hide() {
  hide();
  emit Deactivated();
}

void MainWindow::SetSearchBoxText(const QString& text) {
  auto box = findChild<SearchBox*>("SearchBox");
  if (box == nullptr) {
    return;
  }

  box->SetText(text);
}

void MainWindow::SetSearchResultDescription(const QString& text) {
  auto list = findChild<SearchResultList*>("SearchResultList");
  if (list == nullptr) {
    return;
  }

  list->UpdateDescription(text);
}

void MainWindow::SetSearchResultTitle(const QString& text) {
  auto list = findChild<SearchResultList*>("SearchResultList");
  if (list == nullptr) {
    return;
  }

  list->UpdateTitle(text);
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

void MainWindow::SetHeight(SearchResultList* list) {
  // The minimum height of the window being already set prevents the window from
  // shrinking down all the way to 0, which is a good thing.
  auto list_height = list->Height();
  resize(width(), list_height == 0 ? 0 : minimumHeight() + list_height);
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
