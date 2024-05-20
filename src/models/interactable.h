#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <QKeyCombination>
#include <QString>

class Interactable {
 public:
  virtual QString DragAndDrop() = 0;
  virtual void ProcessKeyPress(const QKeyCombination& combination) = 0;
  virtual void ProcessKeyRelease(const QKeyCombination& combination) = 0;
};

#endif  // INTERACTABLE_H
