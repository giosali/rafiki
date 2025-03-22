#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QObject>
#include <QString>
#include <QtEnvironmentVariables>
#include <QtSystemDetection>
#include <memory>

#include "core/client.h"
#include "core/server.h"
#include "core/settings.h"
#include "ui/mainwindow.h"

void ProcessCommandLineArguments(const QStringList& arguments,
                                 bool from_server) {
  auto parser = QCommandLineParser{};

  // Sets up default options.
  parser.addHelpOption();
  parser.addVersionOption();

  // Sets up custom options.
  auto toggle_option = QCommandLineOption{
    "toggle", "Toggles the visibility of the main input window."};
  parser.addOption(toggle_option);
  auto quit_option = QCommandLineOption{"quit", "Quits the application."};
  parser.addOption(quit_option);

  // It's necessary to parse arguments this way rather than use
  // `parser.process(app)` because the `process` function won't handle
  // arguments passed via the local socket.
  if (!parser.parse(arguments)) {
    return;
  }

  if (parser.isSet("help")) {
    parser.showHelp();
  } else if (parser.isSet("version")) {
    parser.showVersion();
  }

  // The custom options shouldn't be handled if the args aren't from the server.
  if (!from_server) {
    return;
  }

  if (parser.isSet(toggle_option)) {
    for (const auto& widget : QApplication::topLevelWidgets()) {
      if (auto main_window = qobject_cast<MainWindow*>(widget);
          main_window != nullptr) {
        main_window->ToggleVisibility();
      }
    }
  }

  if (parser.isSet(quit_option)) {
    QCoreApplication::quit();
  }
}

int main(int argc, char* argv[]) {
  // https://doc.qt.io/qt-5/qtglobal.html
  // This MUST be called before initializing QApplication.
#ifdef Q_OS_LINUX
  // Ensures application runs on XWayland on Linux.
  qputenv("QT_QPA_PLATFORM", "xcb");
#endif

  auto a = QApplication{argc, argv};
  auto server = Server{};
  auto client = std::make_unique<Client>();

  QObject::connect(&server, &Server::MessageReceived,
                   ProcessCommandLineArguments);
  // Handles unexpected crashes/forcequits.
  QObject::connect(client.get(), &Client::ConnectionRefused, &server,
                   &Server::Remove);

  // Pings the server to deliberately check for a socket error. If a socket
  // error is processed indicating that the application unexpectedly quit, then
  // the previous server will be removed.
  // This is intended to always produce a socket error.
  client->Connect(QApplication::arguments().join(' '));

  // Attempts to start the server. This is what prevents multiple instances of
  // the application.
  if (!server.Listen()) {
    return 0;
  }

  client.reset();

  auto application_name = QString{PROJECT_NAME};
  QApplication::setApplicationName(application_name);
  QApplication::setApplicationVersion(PROJECT_VERSION);
  QApplication::setOrganizationName(application_name);
  QApplication::setOrganizationDomain(application_name + ".com");
  QApplication::setQuitOnLastWindowClosed(false);

  auto paths = QIcon::fallbackSearchPaths();
  if (auto path = QString{"/usr/share/pixmaps"}; !paths.contains(path)) {
    paths.append(path);
    QIcon::setFallbackSearchPaths(paths);
  }

  // Settings must be initialized before initializing MainWindow.
  auto& settings = Settings::GetInstance();
  settings.Initialize();

  auto w = MainWindow{};
  w.CreateTrayIcon();
  w.Show();
  return a.exec();
}
