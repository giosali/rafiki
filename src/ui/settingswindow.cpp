#include "settingswindow.h"

#include "./ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{std::make_unique<Ui::SettingsWindow>()} {
  ui_->setupUi(this);
}

SettingsWindow::~SettingsWindow() {}
