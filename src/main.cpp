#include <singleapplication.h>

#include <QCoreApplication>
#include <QDebug>
#include <QLocale>
#include <QObject>
#include <QString>
#include <QTranslator>
#include <Qt>
#include <QtGlobal>

#include "commandline.h"
#include "mainwindow.h"
#include "messagereceiver.h"

int main(int argc, char *argv[]) {
  // FMI: https://doc.qt.io/qt-5/qtglobal.html
#ifdef Q_OS_LINUX
  // Ensures application runs on Xwayland on Linux.
  qputenv("QT_QPA_PLATFORM", "xcb");
#endif

  auto a = SingleApplication(argc, argv, true);
  QCoreApplication::setApplicationVersion(PROJECT_VERSION);

  auto message = QCoreApplication::arguments().join(" ").toUtf8();

  // Sends commandline arguments to the primary instance of the application from
  // the secondary instance of the application. It then quits the secondary
  // instance.
  if (a.isSecondary()) {
    qDebug() << "Secondary instance: sending message and quitting";
    a.sendMessage(message);
    return 0;
  }

  // Sets application to listen for and process commandline arguments.
  auto mr = MessageReceiver();
  QObject::connect(&a, &SingleApplication::receivedMessage, &mr,
                   &MessageReceiver::ReceivedMessage);

  auto command_line = CommandLine();
  command_line.Parse(message);

  // QTranslator opens translation files, for example, in the following order:
  // 1. /opt/foolib/foo.fr_ca.qm
  // 2. /opt/foolib/foo.fr_ca
  // 3. /opt/foolib/foo.fr.qm
  // 4. /opt/foolib/foo.fr
  // 5. /opt/foolib/foo.qm
  // 6. /opt/foolib/foo
  auto translator = QTranslator();
  for (const auto &locale : QLocale::system().uiLanguages()) {
    const auto base_name = QString(PROJECT_NAME) + "_" + QLocale(locale).name();
    if (!translator.load(":/i18n/" + base_name)) {
      continue;
    }

    a.installTranslator(&translator);
    break;
  }

  auto w = MainWindow();
  w.show();

  return a.exec();
}
