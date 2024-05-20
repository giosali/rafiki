#ifndef TRASH_H
#define TRASH_H

#include <QKeyCombination>
#include <QString>
#include <QWidget>
#include <filesystem>
#include <string>

#include "../core/definitions.h"
#include "../core/project.h"
#include "baseresult.h"

class Trash : public BaseResult {
 public:
  Trash();

  QString DragAndDrop() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  static const inline QString kAltTitle{};
  static const inline bool kAppendSpaceToCommand = false;
  static const inline QString kCommand{"empty trash"};
  static const inline QString kDescription{};
  static const inline QString kIcon{
    Project::GetImageFilePath(defs::ImageFile::kTrash)};
  static const inline QString kId{"9056b806-9219-4f6a-b180-bb087b892bae"};
  static const inline QString kTitle{"Empty trash"};
  static const inline QString kTitlePlaceholder{};

  void Empty() const;

  std::filesystem::path path_;
};

#endif  // TRASH_H
