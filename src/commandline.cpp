#include "commandline.h"

#include <QCommandLineOption>
#include <QCommandLineParser>

#include "mainwindow.h"

void CommandLine::Parse(const QByteArray &line) const {
  QCommandLineParser parser;

  // Sets up builtin options.
  parser.addHelpOption();
  parser.addVersionOption();

  // Sets up custom options.
  QCommandLineOption toggleOption(
      "toggle", "Toggles the visibility of the main input window.");
  parser.addOption(toggleOption);

  QStringList arguments = QString(line).split(" ");
  parser.process(arguments);

  if (parser.isSet(toggleOption)) {
    HandleToggle();
  }
}

void CommandLine::HandleToggle() const {
  QMainWindow *main_window = MainWindow::FindMainWindow();
  if (main_window == nullptr) {
    return;
  }

  main_window->setVisible(main_window->isVisible() ? false : true);
}
