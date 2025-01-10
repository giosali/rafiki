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

  // Populates combo box containing languages and sets user's current language.
  auto user_language = settings.GetLanguage();
  const auto languages = GetSupportedLanguages();
  for (size_t i = 0; i < languages.size(); ++i) {
    auto language = languages[i];
    ui_->language_combo_box->addItem(QLocale::languageToString(language));

    if (user_language == language) {
      ui_->language_combo_box->setCurrentIndex(i);
    }
  }

  // Populates combo box containing territories and sets user's current
  // territory.
  auto user_territory = settings.GetTerritory();
  const auto territories = GetSupportedTerritories();
  for (size_t i = 0; i < territories.size(); ++i) {
    auto territory = territories[i];
    ui_->territory_combo_box->addItem(QLocale::territoryToString(territory));

    if (user_territory == territory) {
      ui_->territory_combo_box->setCurrentIndex(i);
    }
  }

  if (auto theme = settings.GetThemeFilename(); theme == "dark-theme.json") {
    ui_->theme_combo_box->setCurrentIndex(0);
  } else if (theme == "light-theme.json") {
    ui_->theme_combo_box->setCurrentIndex(1);
  }

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

void GeneralTab::UpdateLanguage(int index) {
  auto& settings = Settings::GetInstance();
  settings.SetLanguage(GetSupportedLanguages()[index]);
  settings.Save();
}

void GeneralTab::UpdateTerritory(int index) {
  auto territory = GetSupportedTerritories()[index];
  auto& settings = Settings::GetInstance();
  settings.SetTerritory(territory);
  settings.Save();
  qDebug() << territory;
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
