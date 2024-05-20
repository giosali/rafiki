#ifndef APPLICATION_H
#define APPLICATION_H

#include <QKeyCombination>
#include <QString>

#include "baseresult.h"

class Application : public BaseResult {
 public:
  explicit Application(const QString& name, const QString& icon,
                       uintmax_t icon_size, const QString& description,
                       const QString& exec);

  QString DragAndDrop() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  static inline QString kAltDescription{"Reveal in folder"};

  QString exec_;
};

#endif  // APPLICATION_H
