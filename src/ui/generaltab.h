#pragma once

#include <QLocale>
#include <QWidget>
#include <Qt>
#include <memory>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class GeneralTab;
}
QT_END_NAMESPACE

class GeneralTab : public QWidget {
  Q_OBJECT

 public:
  GeneralTab(QWidget* parent = nullptr);

  ~GeneralTab();

 private:
  std::vector<QLocale::Language> GetSupportedLanguages() const;
  std::vector<QLocale::Territory> GetSupportedTerritories() const;

  std::unique_ptr<Ui::GeneralTab> ui_;

 private slots:
  void TranslateUi();
  void UpdateLanguage(int index);
  void UpdateTerritory(int index);
  void UpdateTheme(int index);
};
