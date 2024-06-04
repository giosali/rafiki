#ifndef TRASH_H
#define TRASH_H

#include <QKeyCombination>
#include <QString>
#include <filesystem>

#include "../core/definitions.h"
#include "../core/io.h"
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
  static const inline QString kAltTitle{};
  static const inline bool kAppendSpaceToCommand = false;
  static const inline QString kCommand{"empty trash"};
  static const inline QString kDescription{};
  static const inline QString kIcon{
    Io::GetImageFilePath(defs::ImageFile::kTrash)};
  static const inline QString kId{"9056b806-9219-4f6a-b180-bb087b892bae"};
  static const inline QString kTitle{"Empty trash"};
  static const inline QString kTitlePlaceholder{};

  void Empty() const;

  std::filesystem::path path_;
};

#endif  // TRASH_H
