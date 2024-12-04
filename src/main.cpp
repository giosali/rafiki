#include <QApplication>
#include <QLocale>
#include <QObject>
#include <QString>
#include <QTranslator>
#include <QtEnvironmentVariables>
#include <QtSystemDetection>
#include <memory>

#include "core/client.h"
#include "core/indexer.h"
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

  auto w = MainWindow{};

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
  client->Connect(QApplication::arguments().join(" "));

  // Attempts to start the server.
  if (!server.Listen()) {
    return 0;
  }

  client.reset();

  auto filename = QString{":/translations/%1_%2.qm"}
                    .arg(application_name.toLower())
                    .arg(Settings::GetInstance().GetLocale());
  if (auto translator = QTranslator{}; translator.load(filename)) {
    a.installTranslator(&translator);
  }

  auto& indexer = Indexer::GetInstance();
  indexer.Initialize();

  w.show();
  return a.exec();
}
