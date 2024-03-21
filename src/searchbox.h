#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QEvent>
#include <QKeyCombination>
#include <QKeyEvent>
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

  int Height() const;

 public slots:
  void Clear();
  void SetText(const QString& text);
  void SimulateKeyPress(QEvent* event);

 signals:
  void KeyPressed(const QKeyCombination& key_combination);
  void KeyReleased(const QKeyCombination& key_combination);
  void TextChanged(const QString& text);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

 private:
  std::unique_ptr<Ui::SearchBox> ui_;
};

#endif  // SEARCHBOX_H
