#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <QKeyCombination>
#include <QObject>
#include <QString>

class Interactable {
 public:
  virtual QString DragAndDrop() = 0;
  virtual void ProcessKeyPress(const QKeyCombination& combination,
                               QObject* parent) = 0;
  virtual void ProcessKeyRelease(const QKeyCombination& combination,
                                 QObject* parent) = 0;
};

#endif  // INTERACTABLE_H
