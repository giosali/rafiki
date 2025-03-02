#include "settingswindow.h"

#include "../core/indexer.h"
#include "../core/settings.h"
#include "./ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{std::make_unique<Ui::SettingsWindow>()} {
  ui_->setupUi(this);

  // Ensures the window is deleted and all resources released upon closure.
  setAttribute(Qt::WA_DeleteOnClose);

  auto& settings = Settings::GetInstance();
  connect(&settings, &Settings::LocaleChanged, this,
          &SettingsWindow::TranslateUi);
  connect(&settings, &Settings::LocaleChanged, ui_->calculator_tab,
          &CalculatorTab::TranslateUi);
  connect(&settings, &Settings::LocaleChanged, ui_->file_search_tab,
          &FileSearchTab::TranslateUi);
  connect(&settings, &Settings::LocaleChanged, ui_->miscellaneous_tab,
          &MiscellaneousTab::TranslateUi);
  connect(&settings, &Settings::LocaleChanged, ui_->web_searches_tab,
          &WebSearchesTab::TranslateUi);
  connect(&settings, &Settings::LocaleChanged, ui_->your_web_searches_tab,
          &YourWebSearchesTab::TranslateUi);

  connect(ui_->calculator_tab, &CalculatorTab::ModelToggled, this,
          &SettingsWindow::ToggleModel);
  connect(ui_->file_search_tab, &FileSearchTab::ModelToggled, this,
          &SettingsWindow::ToggleModel);
  connect(ui_->miscellaneous_tab, &MiscellaneousTab::ModelToggled, this,
          &SettingsWindow::ToggleModel);
  connect(ui_->web_searches_tab, &WebSearchesTab::ModelToggled, this,
          &SettingsWindow::ToggleModel);
  connect(ui_->your_web_searches_tab, &YourWebSearchesTab::ModelToggled, this,
          &SettingsWindow::ToggleModel);
}

SettingsWindow::~SettingsWindow() {}

void SettingsWindow::ToggleModel(Qt::CheckState state, uint64_t id) const {
  auto model = Indexer::GetInstance().GetModel(id);
  auto& settings = Settings::GetInstance();
  switch (state) {
    case Qt::Unchecked:
      model->SetIsEnabled(false);
      settings.AddDisabledFeatureModelId(model->GetId());
      break;
    case Qt::Checked:
      model->SetIsEnabled(true);
      settings.RemoveDisabledFeatureModelId(model->GetId());
      break;
    default:
      break;
  }

  settings.Save();
}

void SettingsWindow::TranslateUi() { ui_->retranslateUi(this); }
