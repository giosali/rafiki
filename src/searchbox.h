#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchBox;
}
QT_END_NAMESPACE

class SearchBox : public QWidget {
  Q_OBJECT

 public:
  explicit SearchBox(QWidget* parent = nullptr);

  ~SearchBox();

 public slots:
  void TextChanged(const QString& text);

 private:
  std::unique_ptr<Ui::SearchBox> ui;
};

#endif  // SEARCHBOX_H
