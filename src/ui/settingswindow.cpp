#include "settingswindow.h"

#include "../core/indexer.h"
#include "../core/settings.h"
#include "./ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{std::make_unique<Ui::SettingsWindow>()} {
  ui_->setupUi(this);

  // Ensures the window is deleted and all resources released upon closure.
  setAttribute(Qt::WA_DeleteOnClose);

  connect(ui_->file_search_tab, &FileSearchTab::ModelToggled, this,
          &SettingsWindow::ToggleModel);
  connect(ui_->miscellaneous_tab, &MiscellaneousTab::ModelToggled, this,
          &SettingsWindow::ToggleModel);
  connect(ui_->web_searches_tab, &WebSearchesTab::WebSearchToggled, this,
          &SettingsWindow::ToggleModel);
  connect(ui_->your_web_searches_tab, &YourWebSearchesTab::WebSearchToggled,
          this, &SettingsWindow::ToggleModel);
}

SettingsWindow::~SettingsWindow() {}

void SettingsWindow::DisableModel(FeatureModel* model) const {
  model->SetIsEnabled(false);

  auto& settings = Settings::GetInstance();
  settings.AddDisabledFeatureModelId(model->GetId());
  settings.Save();
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
