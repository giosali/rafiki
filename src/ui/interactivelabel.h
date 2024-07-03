#ifndef INTERACTIVELABEL_H
#define INTERACTIVELABEL_H

#include <QEnterEvent>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>

class InteractiveLabel : public QLabel {
  Q_OBJECT

 public:
  InteractiveLabel(QWidget* parent = nullptr);

  ~InteractiveLabel();

 signals:
  void Clicked();
  void MouseEntered();
  void MouseLeft();

 protected:
  void enterEvent(QEnterEvent* event) override;
  void leaveEvent(QEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
};

#endif  // INTERACTIVELABEL_H
