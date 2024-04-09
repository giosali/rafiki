#ifndef TRASH_H
#define TRASH_H

#include <QKeyCombination>
#include <QString>
#include <QWidget>
#include <string>

#include "baseresult.h"
#include "input.h"
#include "project.h"

class Trash : public BaseResult {
 public:
  Trash();

  QString DragAndDrop() override;
  bool ProcessInput(const Input& input) override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       QWidget* parent) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         QWidget* parent) override;

 private:
  static const inline QString kAltTitle{};
  static const inline QString kCommand{"empty trash"};
  static const inline QString kDescription{};
  static const inline QString kIcon{
    Project::GetImageFilePath(defs::ImageFile::kTrash)};
  static const inline QString kId{"9056b806-9219-4f6a-b180-bb087b892bae"};
  static const inline QString kPlaceholder{};
  static const inline QString kTitle{"Empty trash"};

  void Empty() const;

  std::string trash_loc_;
};

#endif  // TRASH_H
