#ifndef FILESYSTEMENTRY_H
#define FILESYSTEMENTRY_H

#include <QChar>
#include <QKeyCombination>
#include <QString>
#include <filesystem>
#include <string>
#include <vector>

#include "../core/definitions.h"
#include "../core/finder.h"
#include "../core/input.h"
#include "../core/io.h"
#include "processedresultbuilder.h"

class FileSystemEntry : public ProcessedResultBuilder {
  Q_OBJECT

 public:
  FileSystemEntry();
  explicit FileSystemEntry(const std::filesystem::path& path);

  bool ProcessInput(const Input& input) override;

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  static const inline QString kAltDescription{"Show in folder"};
  static const inline QString kAltTitle{};
  static const inline bool kAppendSpaceToCommand = false;
  static const inline QString kCommand{};
  static const inline QString kDescription{
    "Search for files on your PC and open"};
  static const inline QString kIcon{
    Io::GetImageFilePath(defs::ImageFile::kFileSystemEntry)};
  static const inline QString kId{"397662aa-d686-48e3-b454-2d059cbd7ea0"};
  static const inline QChar kInternalCommand{'\''};
  static const inline QString kTitle{"Open file"};
  static const inline QString kTitlePlaceholder{};

  Finder finder_;
  bool is_entry_;
};

#endif  // FILESYSTEMENTRY_H
