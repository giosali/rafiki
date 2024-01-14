#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Centers the window.
    move(screen()->geometry().center() - frameGeometry().center());

    // Removes space between Central Widget and the window.
    // FMI: https://stackoverflow.com/a/24240025/18831815
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    statusBar()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
