#include "settingswindow.h"

#include <QAbstractButton>
#include <QCheckBox>
#include <QFile>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QJsonArray>
#include <QLabel>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QTableWidgetItem>

#include "../core/file.h"
#include "../core/indexer.h"
#include "../core/models/websearchmodel.h"
#include "../core/paths.h"
#include "../core/settings.h"
#include "./ui_settingswindow.h"
#include "websearchdialog.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{std::make_unique<Ui::SettingsWindow>()} {
  ui_->setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose);

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

  connect(ui_->yourWebSearchesTableWidget, &QTableWidget::itemSelectionChanged,
          this, &SettingsWindow::SetEnabledButtons);
  connect(ui_->editWebSearchButton, &QAbstractButton::clicked, this,
          &SettingsWindow::EditWebSearch);
  connect(ui_->addWebSearchButton, &QAbstractButton::clicked, this,
          &SettingsWindow::AddWebSearch);
  connect(ui_->deleteWebSearchButton, &QAbstractButton::clicked, this,
          &SettingsWindow::DeleteWebSearch);
  connect(ui_->file_search_tab, &FileSearchTab::ModelDisabled, this,
          &SettingsWindow::DisableModel);
  connect(ui_->file_search_tab, &FileSearchTab::ModelEnabled, this,
          &SettingsWindow::EnableModel);

  LoadWebSearches();
  SetEnabledButtons();
}

SettingsWindow::~SettingsWindow() {}

void SettingsWindow::AddWebSearch(bool checked) const {
  OpenWebSearchDialog(0);
}

void SettingsWindow::DeleteWebSearch(bool checked) const {
  auto items = ui_->yourWebSearchesTableWidget->selectedItems();
  for (const auto& item : items) {
    auto data = item->data(Qt::UserRole);
    if (!data.isNull()) {
      auto id = uint64_t{data.toULongLong()};

      auto& settings = Settings::GetInstance();
      settings.RemoveDisabledFeatureModelId(id);
      settings.RemoveUseCount(id);
      settings.Save();

      auto& indexer = Indexer::GetInstance();
      QFile::moveToTrash(indexer.GetModel(id)->GetIconPath());
      indexer.DeleteModel(id);

      SaveWebSearches();

      // Refreshes table UI.
      ClearWebSearches();
      LoadWebSearches();
      break;
    }
  }
}

void SettingsWindow::DisableModel(FeatureModel* model) const {
  model->SetIsEnabled(false);

  auto& settings = Settings::GetInstance();
  settings.AddDisabledFeatureModelId(model->GetId());
  settings.Save();
}

void SettingsWindow::EditWebSearch(bool checked) const {
  for (const auto& item : ui_->yourWebSearchesTableWidget->selectedItems()) {
    if (auto data = item->data(Qt::UserRole); !data.isNull()) {
      OpenWebSearchDialog(data.toULongLong());
      break;
    }
  }
}

void SettingsWindow::EnableModel(FeatureModel* model) const {
  model->SetIsEnabled(true);

  auto& settings = Settings::GetInstance();
  settings.RemoveDisabledFeatureModelId(model->GetId());
  settings.Save();
}

void SettingsWindow::ToggleModel(Qt::CheckState state, uint64_t id) const {
  auto model = Indexer::GetInstance().GetModel(id);
  switch (state) {
    case Qt::Unchecked:
      DisableModel(model);
      break;
    case Qt::Checked:
      EnableModel(model);
      break;
    default:
      break;
  }
}

void SettingsWindow::SetEnabledButtons() const {
  auto current_row = ui_->yourWebSearchesTableWidget->currentRow();
  if (current_row == -1) {
    ui_->editWebSearchButton->setEnabled(false);
    ui_->deleteWebSearchButton->setEnabled(false);
  } else {
    ui_->editWebSearchButton->setEnabled(true);
    ui_->deleteWebSearchButton->setEnabled(true);
  }
}

void SettingsWindow::ClearWebSearches() const {
  ui_->webSearchesTableWidget->clearContents();
  ui_->webSearchesTableWidget->setRowCount(0);

  ui_->yourWebSearchesTableWidget->clearContents();
  ui_->yourWebSearchesTableWidget->setRowCount(0);
}

void SettingsWindow::LoadWebSearches() const {
  auto& indexer = Indexer::GetInstance();
  auto web_search_models = indexer.GetModels<WebSearchModel>();
  for (const auto& model : web_search_models) {
    auto table_widget = model->GetIsCustom() ? ui_->yourWebSearchesTableWidget
                                             : ui_->webSearchesTableWidget;
    auto row_count = table_widget->rowCount();
    table_widget->setRowCount(row_count + 1);

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
    table_widget->setCellWidget(row_count, 0, icon_widget);

    auto id = model->GetId();

    // Command column
    auto command_item = new QTableWidgetItem{model->GetCommand()};
    command_item->setData(Qt::UserRole, QString::number(id));  // Stores id
    command_item->setTextAlignment(Qt::AlignCenter);
    table_widget->setItem(row_count, 1, command_item);

    // Example column.
    auto title = model->GetTitle();
    auto example_item = new QTableWidgetItem{
      title.contains("%1") ? title.arg(model->GetTitlePlaceholder()) : title};
    table_widget->setItem(row_count, 2, example_item);

    // Enabled column.
    auto check_box_widget = new QWidget{};
    auto check_box_layout = new QHBoxLayout{check_box_widget};
    auto check_box = new QCheckBox{};
    check_box->setCheckState(model->GetIsEnabled() ? Qt::Checked
                                                   : Qt::Unchecked);

    connect(check_box, &QCheckBox::checkStateChanged,
            [this, id](Qt::CheckState state) { ToggleModel(state, id); });

    check_box_layout->addWidget(check_box);
    check_box_layout->setAlignment(Qt::AlignCenter);
    check_box_layout->setContentsMargins(0, 0, 0, 0);
    check_box_widget->setLayout(check_box_layout);
    table_widget->setCellWidget(row_count, 3, check_box_widget);
  }
}

void SettingsWindow::OpenWebSearchDialog(uint64_t id) const {
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

void SettingsWindow::SaveWebSearches() const {
  auto array = QJsonArray{};
  for (const auto& model : Indexer::GetInstance().GetModels<WebSearchModel>()) {
    if (model->GetIsCustom()) {
      array.push_back(model->ToJson());
    }
  }

  File::Write(Paths::GetPath(Paths::Json::kUserWebSearches), array);
}
