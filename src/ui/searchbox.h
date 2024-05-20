#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QEvent>
#include <QKeyCombination>
#include <QKeyEvent>
#include <QString>
#include <QWidget>
#include <memory>

#include "../core/input.h"

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
  Input GetText() const;

 public slots:
  void Clear();
  void SetText(const QString& text);

 signals:
  void KeyPressed(const QKeyCombination& combination);
  void KeyReleased(const QKeyCombination& combination);
  void TextChanged(const Input& input);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

 private:
  std::unique_ptr<Ui::SearchBox> ui_;
};

#endif  // SEARCHBOX_H
