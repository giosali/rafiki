#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <QKeyCombination>
#include <QString>
#include <QWidget>

class Interactable {
 public:
  virtual QString DragAndDrop() = 0;
  virtual void ProcessKeyPress(const QKeyCombination& combination,
                               QWidget* parent) = 0;
  virtual void ProcessKeyRelease(const QKeyCombination& combination,
                                 QWidget* parent) = 0;
};

#endif  // INTERACTABLE_H
