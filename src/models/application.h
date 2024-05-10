#ifndef APPLICATION_H
#define APPLICATION_H

#include <QKeyCombination>
#include <QSettings>
#include <QString>
#include <QWidget>

#include "baseresult.h"

class Application : public BaseResult {
 public:
  explicit Application(const QString& name, const QString& icon,
                       const QString& description, const QString& exec);

  QString DragAndDrop() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       QWidget* parent) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         QWidget* parent) override;

 private:
  static inline QString kAltDescription{"Reveal in folder"};

  QString exec_;
};

#endif  // APPLICATION_H
