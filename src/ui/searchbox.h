#pragma once

#include <QKeyCombination>
#include <QString>
#include <QWidget>
#include <memory>

#include "../core/theme.h"
#include "customlineedit.h"

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

  QString GetText() const;
  int Height() const;

 public slots:
  void ApplyTheme(Theme* theme);
  void Clear();
  void SetText(const QString& text);

 signals:
  void KeyPressed(const QKeyCombination& combination);
  void KeyReleased(const QKeyCombination& combination);
  void TextChanged(const QString& text);

 private:
  std::unique_ptr<Ui::SearchBox> ui_;

 private slots:
  void ProcessText(const QString& text);
};
