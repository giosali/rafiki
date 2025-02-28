#pragma once

#include <memory>

#include "toggletab.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WebSearchesTab;
}
QT_END_NAMESPACE

class WebSearchesTab : public ToggleTab {
  Q_OBJECT

 public:
  WebSearchesTab(QWidget* parent = nullptr);

  ~WebSearchesTab();

 private:
  void ClearWebSearches() const;
  void LoadWebSearches();

  std::unique_ptr<Ui::WebSearchesTab> ui_;
};
