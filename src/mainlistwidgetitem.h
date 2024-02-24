#ifndef MAINLISTWIDGETITEM_H
#define MAINLISTWIDGETITEM_H

#include <QWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainListWidgetItem;
}
QT_END_NAMESPACE

class MainListWidgetItem : public QWidget {
  Q_OBJECT

 public:
  explicit MainListWidgetItem(QWidget* parent = nullptr);

  ~MainListWidgetItem();

  void setIcon(const QString& text) const;

 private:
  std::unique_ptr<Ui::MainListWidgetItem> ui;
};

#endif  // MAINLISTWIDGETITEM_H
