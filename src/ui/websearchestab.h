#pragma once

#include <QWidget>
#include <Qt>
#include <cstdint>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class WebSearchesTab;
}
QT_END_NAMESPACE

class WebSearchesTab : public QWidget {
  Q_OBJECT

 public:
  WebSearchesTab(QWidget* parent = nullptr);

  ~WebSearchesTab();

 signals:
  void ModelToggled(Qt::CheckState state, uint64_t id);

 private:
  void ClearWebSearches() const;
  void LoadWebSearches();

  std::unique_ptr<Ui::WebSearchesTab> ui_;
};
