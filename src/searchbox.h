#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QString>
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
  void SetText(const QString& text);

 signals:
  void TextChanged(const QString& text);

 private:
  std::unique_ptr<Ui::SearchBox> ui_;
  QString text_;
};

#endif  // SEARCHBOX_H
