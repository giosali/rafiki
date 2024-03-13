#include "commandline.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <Qt>
#include <QtGlobal>

#include "mainwindow.h"
// #include "singleapplication.h"

void CommandLine::Parse(const QByteArray &line) const {
  qDebug() << "Parsing args";
  QCommandLineParser parser;

  // Sets up builtin options.
  parser.addHelpOption();
  parser.addVersionOption();

  // Sets up custom options.
  QCommandLineOption toggleOption(
      "toggle", "Toggles the visibility of the main input window.");
  parser.addOption(toggleOption);
  QCommandLineOption quitOption("quit", "Quits the application.");
  parser.addOption(quitOption);

  QStringList arguments = QString(line).split(" ");
  parser.process(arguments);

  if (parser.isSet(toggleOption)) {
    HandleToggle();
  }

  if (parser.isSet(quitOption)) {
    HandleQuit();
  }
}

void CommandLine::HandleToggle() const {
  QMainWindow *main_window = MainWindow::FindMainWindow();
  if (main_window == nullptr) {
    return;
  }

  if (main_window->isVisible()) {
    qDebug() << "Hiding main window";
    main_window->hide();
  } else {
    qDebug() << "Showing main window";
    main_window->show();
    main_window->activateWindow();

    // If you want to ensure that the window is stacked on top as well after
    // calling activateWindow(), you should also call raise().
    main_window->raise();
  }
}

void CommandLine::HandleQuit() const {
  qDebug() << "Qutting application";
  // SingleApplication::instance()->quit();
}
