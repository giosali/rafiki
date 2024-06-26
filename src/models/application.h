#ifndef APPLICATION_H
#define APPLICATION_H

#include <QKeyCombination>
#include <QString>

#include "result.h"

class Application : public Result {
  Q_OBJECT

 public:
  explicit Application(const QString& name, const QString& icon,
                       uintmax_t icon_size, const QString& description,
                       const QString& exec);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  static const QString kAltDescription;

  QString exec_;
};

#endif  // APPLICATION_H
