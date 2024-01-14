#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtGlobal>

int main(int argc, char *argv[])
{
#ifdef Q_OS_LINUX
    // FMI: https://doc.qt.io/qt-5/qtglobal.html
    //
    // Ensures application runs on Xwayland on Linux.
    qputenv("QT_QPA_PLATFORM", "xcb");
#endif

    QApplication a(argc, argv);

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
