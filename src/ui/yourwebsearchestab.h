#pragma once

#include <memory>

#include "toggletab.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class YourWebSearchesTab;
}
QT_END_NAMESPACE

class YourWebSearchesTab : public ToggleTab {
  Q_OBJECT

 public:
  YourWebSearchesTab(QWidget* parent = nullptr);

  ~YourWebSearchesTab();

 public slots:
  void TranslateUi();

 private:
  void ClearWebSearches() const;
  void LoadWebSearches();
  void OpenWebSearchDialog(uint64_t id = 0);
  void SaveWebSearches() const;

  std::unique_ptr<Ui::YourWebSearchesTab> ui_;

 private slots:
  void AddWebSearch(bool checked);
  void DeleteWebSearch(bool checked);
  void EditWebSearch(bool checked);
  void UpdateButtons() const;
};
