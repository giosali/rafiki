#pragma once

#include <QWidget>
#include <Qt>
#include <cstdint>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class YourWebSearchesTab;
}
QT_END_NAMESPACE

class YourWebSearchesTab : public QWidget {
  Q_OBJECT

 public:
  YourWebSearchesTab(QWidget* parent = nullptr);

  ~YourWebSearchesTab();

 signals:
  void ModelToggled(Qt::CheckState state, uint64_t id);

 private:
  void ClearWebSearches() const;
  void LoadWebSearches();
  void OpenWebSearchDialog(uint64_t id);
  void SaveWebSearches() const;

  std::unique_ptr<Ui::YourWebSearchesTab> ui_;

 private slots:
  void AddWebSearch(bool checked);
  void DeleteWebSearch(bool checked);
  void EditWebSearch(bool checked);
  void UpdateButtons() const;
};
