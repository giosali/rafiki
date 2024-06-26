#ifndef TRASH_H
#define TRASH_H

#include <QKeyCombination>
#include <QString>
#include <cstdint>
#include <filesystem>

#include "baseresult.h"

class Trash : public BaseResult {
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
  static const QString kAltTitle;
  static const bool kAppendSpaceToCommand;
  static const QString kCommand;
  static const QString kDescription;
  static const QString kIcon;
  static const uint64_t kId;
  static const QString kTitle;
  static const QString kTitlePlaceholder;

  void Empty() const;

  std::filesystem::path path_;
};

#endif  // TRASH_H
