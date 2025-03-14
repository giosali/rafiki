#pragma once

#include <QKeyCombination>
#include <QKeyEvent>
#include <QLineEdit>

class CustomLineEdit : public QLineEdit {
  Q_OBJECT

 public:
  CustomLineEdit(QWidget* parent = nullptr);

  ~CustomLineEdit();

 signals:
  void KeyPressed(const QKeyCombination& combination);
  void KeyReleased(const QKeyCombination& combination);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
};
