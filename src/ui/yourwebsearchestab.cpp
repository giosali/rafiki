#include "yourwebsearchestab.h"

#include <QAbstractButton>
#include <QCheckBox>
#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QLabel>
#include <QString>
#include <QStringList>
#include <QTableWidgetItem>

#include "../core/file.h"
#include "../core/indexer.h"
#include "../core/models/websearchmodel.h"
#include "../core/settings.h"
#include "./ui_yourwebsearchestab.h"
#include "websearchdialog.h"

YourWebSearchesTab::YourWebSearchesTab(QWidget* parent)
    : QWidget{parent}, ui_{std::make_unique<Ui::YourWebSearchesTab>()} {
  ui_->setupUi(this);

  auto columns = QStringList{"Icon", "Command", "Example", "Enabled"};

  ui_->table->setColumnCount(columns.size());
  ui_->table->verticalHeader()->hide();
  ui_->table->setHorizontalHeaderLabels(columns);
  ui_->table->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  ui_->table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

  connect(ui_->table, &QTableWidget::itemSelectionChanged, this,
          &YourWebSearchesTab::UpdateButtons);
  connect(ui_->add_button, &QAbstractButton::clicked, this,
          &YourWebSearchesTab::AddWebSearch);
  connect(ui_->delete_button, &QAbstractButton::clicked, this,
          &YourWebSearchesTab::DeleteWebSearch);
  connect(ui_->edit_button, &QAbstractButton::clicked, this,
          &YourWebSearchesTab::EditWebSearch);

  LoadWebSearches();
  UpdateButtons();
}

YourWebSearchesTab::~YourWebSearchesTab() {}

void YourWebSearchesTab::ClearWebSearches() const {
  ui_->table->clearContents();
  ui_->table->setRowCount(0);
}

void YourWebSearchesTab::LoadWebSearches() {
  auto& indexer = Indexer::GetInstance();
  auto web_search_models = indexer.GetModels<WebSearchModel>();
  for (const auto& model : web_search_models) {
    if (!model->GetIsCustom()) {
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

    connect(
      check_box, &QCheckBox::checkStateChanged,
      [this, id](Qt::CheckState state) { emit WebSearchToggled(state, id); });

    check_box_layout->addWidget(check_box);
    check_box_layout->setAlignment(Qt::AlignCenter);
    check_box_layout->setContentsMargins(0, 0, 0, 0);
    check_box_widget->setLayout(check_box_layout);
    ui_->table->setCellWidget(row_count, 3, check_box_widget);
  }
}

void YourWebSearchesTab::OpenWebSearchDialog(uint64_t id) {
  auto dialog = id == 0 ? WebSearchDialog{} : WebSearchDialog{id};
  auto dialog_code = dialog.exec();
  switch (dialog_code) {
    case QDialog::Accepted:
      SaveWebSearches();
      ClearWebSearches();
      LoadWebSearches();
      break;
  }
}

void YourWebSearchesTab::SaveWebSearches() const {
  auto array = QJsonArray{};
  for (const auto& model : Indexer::GetInstance().GetModels<WebSearchModel>()) {
    if (model->GetIsCustom()) {
      array.push_back(model->ToJson());
    }
  }

  File::Write(Paths::GetPath(Paths::Json::kUserWebSearches), array);
}

void YourWebSearchesTab::AddWebSearch(bool checked) { OpenWebSearchDialog(0); }

void YourWebSearchesTab::DeleteWebSearch(bool checked) {
  auto items = ui_->table->selectedItems();
  for (const auto& item : items) {
    auto data = item->data(Qt::UserRole);
    if (!data.isNull()) {
      auto id = uint64_t{data.toULongLong()};

      auto& settings = Settings::GetInstance();
      settings.RemoveDisabledFeatureModelId(id);
      settings.RemoveUseCount(id);
      settings.Save();

      auto& indexer = Indexer::GetInstance();
      auto model = static_cast<WebSearchModel*>(indexer.GetModel(id));
      QFile::moveToTrash(model->GetIconPath());
      indexer.DeleteModel(id);

      SaveWebSearches();

      // Refreshes table UI.
      ClearWebSearches();
      LoadWebSearches();
      break;
    }
  }
}

void YourWebSearchesTab::EditWebSearch(bool checked) {
  for (const auto& item : ui_->table->selectedItems()) {
    if (auto data = item->data(Qt::UserRole); !data.isNull()) {
      OpenWebSearchDialog(data.toULongLong());
      break;
    }
  }
}

void YourWebSearchesTab::UpdateButtons() const {
  if (auto current_row = ui_->table->currentRow(); current_row == -1) {
    ui_->edit_button->setEnabled(false);
    ui_->delete_button->setEnabled(false);
  } else {
    ui_->edit_button->setEnabled(true);
    ui_->delete_button->setEnabled(true);
  }
}
