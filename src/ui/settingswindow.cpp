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

  auto columns = QStringList{"Icon", "Command", "Example", "Enabled"};

  ui_->webSearchesTableWidget->setColumnCount(columns.size());
  ui_->webSearchesTableWidget->verticalHeader()->hide();
  ui_->webSearchesTableWidget->setHorizontalHeaderLabels(columns);
  ui_->webSearchesTableWidget->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  ui_->webSearchesTableWidget->horizontalHeader()->setSectionResizeMode(
    2, QHeaderView::Stretch);

  ui_->yourWebSearchesTableWidget->setColumnCount(columns.size());
  ui_->yourWebSearchesTableWidget->verticalHeader()->hide();
  ui_->yourWebSearchesTableWidget->setHorizontalHeaderLabels(columns);
  ui_->yourWebSearchesTableWidget->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  ui_->yourWebSearchesTableWidget->horizontalHeader()->setSectionResizeMode(
    2, QHeaderView::Stretch);
}

SettingsWindow::~SettingsWindow() {}

void SettingsWindow::closeEvent(QCloseEvent* event) {
  ui_->webSearchesTableWidget->clearContents();
  ui_->webSearchesTableWidget->setRowCount(0);

  ui_->yourWebSearchesTableWidget->clearContents();
  ui_->yourWebSearchesTableWidget->setRowCount(0);

  QWidget::closeEvent(event);
}

void SettingsWindow::showEvent(QShowEvent* event) {
  const auto web_searches = Io::FilterResults<WebSearch>();
  for (const auto& web_search : web_searches) {
    auto table_widget = web_search->IsCustom() ? ui_->yourWebSearchesTableWidget
                                               : ui_->webSearchesTableWidget;
    auto row_count = table_widget->rowCount();
    table_widget->setRowCount(row_count + 1);

    // Icon column.
    auto icon_widget = new QWidget{};
    auto layout = new QHBoxLayout{icon_widget};
    auto icon_label = new QLabel{};
    icon_label->setPixmap(web_search->GetIcon(16));
    layout->addWidget(icon_label);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    icon_widget->setLayout(layout);
    table_widget->setCellWidget(row_count, 0, icon_widget);

    // Command column
    auto command_item = new QTableWidgetItem{web_search->GetCommand()};
    command_item->setTextAlignment(Qt::AlignCenter);
    table_widget->setItem(row_count, 1, command_item);

    // Example column.
    auto example_item =
      new QTableWidgetItem{web_search->FormatTitle(QString{})};
    table_widget->setItem(row_count, 2, example_item);

    // Enabled column.
    auto check_box_widget = new QWidget{};
    auto check_box_layout = new QHBoxLayout{check_box_widget};
    auto check_box = new QCheckBox{};
    check_box->setCheckState(web_search->IsEnabled() ? Qt::Checked
                                                     : Qt::Unchecked);

    auto id = web_search->GetId();

    // TODO: &QCheckBox::stateChanged is deprecated and superseded by
    // &QCheckBox::checkStateChanged in 6.7.
    connect(check_box, &QCheckBox::stateChanged,
            [this, id](int state) { ProcessStateChange(state, id); });

    check_box_layout->addWidget(check_box);
    check_box_layout->setAlignment(Qt::AlignCenter);
    check_box_layout->setContentsMargins(0, 0, 0, 0);
    check_box_widget->setLayout(check_box_layout);
    table_widget->setCellWidget(row_count, 3, check_box_widget);
  }

  QWidget::showEvent(event);
}

void SettingsWindow::ProcessStateChange(int state, uint64_t id) const {
  switch (state) {
    case Qt::Unchecked:
      Io::ToggleResult(id, false);
      break;
    case Qt::Checked:
      Io::ToggleResult(id, true);
      break;
  }
}
