#include <QApplication>
#include <QLocale>
#include <QObject>
#include <QString>
#include <QTranslator>
#include <QtEnvironmentVariables>
#include <QtSystemDetection>
#include <memory>

#include "core/client.h"
#include "core/io.h"
#include "core/server.h"
#include "core/utils.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[]) {
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

  // QTranslator opens translation files, for example, in the following order:
  // 1. /opt/foolib/foo.fr_ca.qm
  // 2. /opt/foolib/foo.fr_ca
  // 3. /opt/foolib/foo.fr.qm
  // 4. /opt/foolib/foo.fr
  // 5. /opt/foolib/foo.qm
  // 6. /opt/foolib/foo
  auto translator = QTranslator{};
  for (const auto &locale : QLocale::system().uiLanguages()) {
    const auto base_name = application_name + "_" + QLocale(locale).name();
    if (!translator.load(":/i18n/" + base_name)) {
      continue;
    }

    a.installTranslator(&translator);
    break;
  }

  Io::Initialize();

  w.show();
  return a.exec();
}
