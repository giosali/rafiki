#include "settingswindow.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QTableWidgetItem>
#include <Qt>

#include "../core/io.h"
#include "./ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{std::make_unique<Ui::SettingsWindow>()} {
  ui_->setupUi(this);

  ui_->tableWidget->setColumnCount(4);
  ui_->tableWidget->verticalHeader()->hide();
  ui_->tableWidget->setHorizontalHeaderLabels(
    QStringList{} << "Icon" << "Command" << "Example" << "Enabled");
  ui_->tableWidget->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  ui_->tableWidget->horizontalHeader()->setSectionResizeMode(
    2, QHeaderView::Stretch);
}

SettingsWindow::~SettingsWindow() {}

void SettingsWindow::closeEvent(QCloseEvent* event) {
  ui_->tableWidget->clearContents();

  QWidget::closeEvent(event);
}

void SettingsWindow::showEvent(QShowEvent* event) {
  const auto web_searches = Io::GetWebSearches();
  for (size_t i = 0; i < web_searches.size(); ++i) {
    auto web_search = web_searches[i];
    ui_->tableWidget->setRowCount(i + 1);

    auto icon_widget = new QWidget{};
    auto layout = new QHBoxLayout{icon_widget};
    auto icon_label = new QLabel{};
    // label->setMaximumSize(16, 16);
    // label->setScaledContents(true);
    icon_label->setPixmap(web_search->GetIcon(16));
    layout->addWidget(icon_label);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    icon_widget->setLayout(layout);
    ui_->tableWidget->setCellWidget(i, 0, icon_widget);

    auto command_item = new QTableWidgetItem{web_search->GetCommand()};
    command_item->setTextAlignment(Qt::AlignCenter);
    ui_->tableWidget->setItem(i, 1, command_item);

    auto example_item =
      new QTableWidgetItem{web_search->FormatTitle(QString{})};
    ui_->tableWidget->setItem(i, 2, example_item);

    auto check_box_widget = new QWidget{};
    auto check_box_layout = new QHBoxLayout{check_box_widget};
    auto check_box = new QCheckBox{};
    check_box_layout->addWidget(check_box);
    check_box_layout->setAlignment(Qt::AlignCenter);
    check_box_layout->setContentsMargins(0, 0, 0, 0);
    check_box_widget->setLayout(check_box_layout);
    ui_->tableWidget->setCellWidget(i, 3, check_box_widget);
  }

  QWidget::showEvent(event);
}
