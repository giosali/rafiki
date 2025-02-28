#include "websearchestab.h"

#include <QCheckBox>
#include <QHeaderView>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QTableWidgetItem>
#include <algorithm>

#include "../core/indexer.h"
#include "../core/models/websearchmodel.h"
#include "./ui_websearchestab.h"

WebSearchesTab::WebSearchesTab(QWidget* parent)
    : ToggleTab{parent}, ui_{std::make_unique<Ui::WebSearchesTab>()} {
  ui_->setupUi(this);

  auto columns = QStringList{"Icon", "Command", "Example", "Enabled"};

  ui_->table->setColumnCount(columns.size());
  ui_->table->verticalHeader()->hide();
  ui_->table->setHorizontalHeaderLabels(columns);
  ui_->table->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  ui_->table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

  LoadWebSearches();
}

WebSearchesTab::~WebSearchesTab() {}

void WebSearchesTab::ClearWebSearches() const {
  ui_->table->clearContents();
  ui_->table->setRowCount(0);
}

void WebSearchesTab::LoadWebSearches() {
  auto models = Indexer::GetInstance().GetModels<WebSearchModel>();
  std::sort(models.begin(), models.end(),
            [](WebSearchModel* model1, WebSearchModel* model2) {
              return model1->GetCommand() < model2->GetCommand();
            });

  for (const auto& model : models) {
    if (model->GetIsCustom()) {
      continue;
    }

    auto row_count = ui_->table->rowCount();
    ui_->table->setRowCount(row_count + 1);

    // Icon column.
    auto icon_widget = new QWidget{};
    auto layout = new QHBoxLayout{icon_widget};
    auto icon_label = new QLabel{};
    icon_label->setPixmap(model->GetIcon().scaled(30, 30, Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
    layout->addWidget(icon_label);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    icon_widget->setLayout(layout);
    ui_->table->setCellWidget(row_count, 0, icon_widget);

    auto id = model->GetId();

    // Command column
    auto command_item = new QTableWidgetItem{model->GetCommand()};
    command_item->setData(Qt::UserRole, QString::number(id));  // Stores id
    command_item->setTextAlignment(Qt::AlignCenter);
    ui_->table->setItem(row_count, 1, command_item);

    // Example column.
    auto title = model->GetTitle();
    auto example_item = new QTableWidgetItem{
      title.contains("%1") ? title.arg(model->GetTitlePlaceholder()) : title};
    ui_->table->setItem(row_count, 2, example_item);

    // Enabled column.
    auto check_box_widget = new QWidget{};
    auto check_box_layout = new QHBoxLayout{check_box_widget};
    auto check_box = new QCheckBox{};
    check_box->setCheckState(model->GetIsEnabled() ? Qt::Checked
                                                   : Qt::Unchecked);

    connect(check_box, &QCheckBox::checkStateChanged,
            [this, id](Qt::CheckState state) { emit ModelToggled(state, id); });

    check_box_layout->addWidget(check_box);
    check_box_layout->setAlignment(Qt::AlignCenter);
    check_box_layout->setContentsMargins(0, 0, 0, 0);
    check_box_widget->setLayout(check_box_layout);
    ui_->table->setCellWidget(row_count, 3, check_box_widget);
  }
}
