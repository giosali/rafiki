#pragma once

#include <QKeyCombination>
#include <QKeyEvent>
#include <QString>
#include <QWidget>
#include <memory>

#include "../core/theme.h"

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
  QString GetText() const;

 public slots:
  void ApplyTheme(Theme* theme);
  void Clear();
  void SetText(const QString& text);

 signals:
  void KeyPressed(const QKeyCombination& combination);
  void KeyReleased(const QKeyCombination& combination);
  void TextChanged(const QString& text);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

 private:
  std::unique_ptr<Ui::SearchBox> ui_;

 private slots:
  void ProcessText(const QString& text);
};
