#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <QKeyCombination>
#include <QObject>
#include <QString>

#include "../core/input.h"

class Interactable : public QObject {
  Q_OBJECT

 public slots:
  virtual void Drag() = 0;
  virtual void ProcessKeyPress(const QKeyCombination& combination,
                               const Input& input) = 0;
  virtual void ProcessKeyRelease(const QKeyCombination& combination,
                                 const Input& input) = 0;

 signals:
  void Dragged(const QString& text);
  void Hidden();
  void NewDescriptionRequested(const QString& text);
  void NewSearchBoxTextRequested(const QString& text);
  void NewTitleRequested(const QString& text);
};

#endif  // INTERACTABLE_H
