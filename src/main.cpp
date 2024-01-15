#include "argumentparser.h"
#include "mainwindow.h"
#include "messagereceiver.h"

#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QObject>
#include <QTranslator>
#include <QtGlobal>

#include <singleapplication.h>

int main(int argc, char *argv[])
{
    // FMI: https://doc.qt.io/qt-5/qtglobal.html
#ifdef Q_OS_LINUX
    // Ensures application runs on Xwayland on Linux.
    qputenv("QT_QPA_PLATFORM", "xcb");
#endif

    SingleApplication a(argc, argv, true);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    QByteArray message = QCoreApplication::arguments().join(" ").toUtf8();
    MessageReceiver mr;

    // Sends commandline arguments to the primary instance of the application from the secondary
    // instance of the application.
    // It then quits the secondary instance.
    if (a.isSecondary()) {
        a.sendMessage(message);
        return 0;
    }

    // Sets application to listen for and process commandline arguments.
    QObject::connect(&a, &SingleApplication::receivedMessage, &mr, &MessageReceiver::receivedMessage);

    ArgumentParser argumentParser;
    argumentParser.parse(message);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "rafiki_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
