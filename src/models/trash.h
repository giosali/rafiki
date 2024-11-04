#ifndef TRASH_H
#define TRASH_H

#include <QKeyCombination>
#include <QString>
#include <filesystem>

#include "result.h"

class Trash : public Result {
  Q_OBJECT

 public:
  Trash();

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  void Empty() const;

  std::filesystem::path path_;
};

#endif  // TRASH_H
