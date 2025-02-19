#include <QApplication>
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

int main(int argc, char* argv[]) {
  // https://doc.qt.io/qt-5/qtglobal.html
#ifdef Q_OS_LINUX
  // Ensures application runs on XWayland on Linux.
  qputenv("QT_QPA_PLATFORM", "xcb");
#endif

  auto a = QApplication{argc, argv};
  auto application_name = QString{PROJECT_NAME};
  QApplication::setApplicationName(application_name);
  QApplication::setApplicationVersion(PROJECT_VERSION);
  QApplication::setOrganizationName(application_name);
  QApplication::setOrganizationDomain(application_name + ".com");
  QApplication::setQuitOnLastWindowClosed(false);

  // Settings must be initialized before initializing MainWindow.
  auto& settings = Settings::GetInstance();
  settings.Initialize();

  auto arguments = QApplication::arguments();
  auto w = MainWindow{};
  w.ProcessCommandLineArguments(arguments, false);

  auto server = Server{&a};
  auto client = std::make_unique<Client>(&a);

  QObject::connect(&server, &Server::MessageReceived, &w,
                   &MainWindow::ProcessCommandLineArguments);
  // Handles unexpected crashes/forcequits.
  QObject::connect(client.get(), &Client::ConnectionRefused, &server,
                   &Server::Remove);

  // Pings the server to deliberately check for a socket error. If a socket
  // error is processed indicating that the application unexpectedly quit, then
  // the previous server will be removed.
  // This is intended to always produce a socket error.
  client->Connect(arguments.join(' '));

  // Attempts to start the server. This is what prevents multiple instances of
  // the application.
  if (!server.Listen()) {
    return 0;
  }

  client.reset();

  auto paths = QIcon::fallbackSearchPaths();
  if (auto path = QString{"/usr/share/pixmaps"}; !paths.contains(path)) {
    paths.append(path);
    QIcon::setFallbackSearchPaths(paths);
  }

  w.CreateTrayIcon();
  w.Show();
  return a.exec();
}
