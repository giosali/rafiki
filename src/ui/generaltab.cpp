#include "generaltab.h"

#include <QComboBox>
#include <QTranslator>

#include "../core/settings.h"
#include "../core/theme.h"
#include "./ui_generaltab.h"

GeneralTab::GeneralTab(QWidget* parent)
    : QWidget{parent}, ui_{std::make_unique<Ui::GeneralTab>()} {
  ui_->setupUi(this);

  auto& settings = Settings::GetInstance();

  // Sets active item to user's current language.
  auto user_language = settings.GetLanguage();
  const auto languages = GetSupportedLanguages();
  for (size_t i = 0; i < languages.size(); ++i) {
    if (user_language == languages[i]) {
      ui_->language_combo_box->setCurrentIndex(i);
      break;
    }
  }

  // Sets active item user's current territory.
  auto user_territory = settings.GetTerritory();
  const auto territories = GetSupportedTerritories();
  for (size_t i = 0; i < territories.size(); ++i) {
    if (user_territory == territories[i]) {
      ui_->territory_combo_box->setCurrentIndex(i);
      break;
    }
  }

  if (auto theme = settings.GetThemeFilename(); theme == "dark-theme.json") {
    ui_->theme_combo_box->setCurrentIndex(0);
  } else if (theme == "light-theme.json") {
    ui_->theme_combo_box->setCurrentIndex(1);
  }

  connect(&settings, &Settings::LocaleChanged, this, &GeneralTab::TranslateUi);
  connect(ui_->language_combo_box, &QComboBox::currentIndexChanged, this,
          &GeneralTab::UpdateLanguage);
  connect(ui_->territory_combo_box, &QComboBox::currentIndexChanged, this,
          &GeneralTab::UpdateTerritory);
  connect(ui_->theme_combo_box, &QComboBox::currentIndexChanged, this,
          &GeneralTab::UpdateTheme);
}

GeneralTab::~GeneralTab() {}

std::vector<QLocale::Language> GeneralTab::GetSupportedLanguages() const {
  return {QLocale::English, QLocale::French};
}

std::vector<QLocale::Territory> GeneralTab::GetSupportedTerritories() const {
  return {QLocale::France, QLocale::UnitedStates};
}

void GeneralTab::TranslateUi() { ui_->retranslateUi(this); }

void GeneralTab::UpdateLanguage(int index) {
  if (index > -1) {
    auto& settings = Settings::GetInstance();
    settings.SetLanguage(GetSupportedLanguages()[index]);
    settings.Save();
  }
}

void GeneralTab::UpdateTerritory(int index) {
  if (index > -1) {
    auto& settings = Settings::GetInstance();
    settings.SetTerritory(GetSupportedTerritories()[index]);
    settings.Save();
  }
}

void GeneralTab::UpdateTheme(int index) {
  auto filename = QString{};
  switch (index) {
    case 0:
      filename = "dark-theme.json";
      break;
    case 1:
      filename = "light-theme.json";
      break;
  }

  if (!filename.isEmpty()) {
    Theme::GetInstance().LoadFile(filename);

    auto& settings = Settings::GetInstance();
    settings.SetThemeFilename(filename);
    settings.Save();
  }
}
