#ifndef FILESYSTEMENTRY_H
#define FILESYSTEMENTRY_H

#include <QString>
#include <filesystem>
#include <string>

#include "../core/definitions.h"
#include "../core/finder.h"
#include "../core/input.h"
#include "../core/project.h"
#include "processedresultbuilder.h"

class FileSystemEntry : public ProcessedResultBuilder {
 public:
  FileSystemEntry();
  explicit FileSystemEntry(const std::filesystem::path& path);

  bool ProcessInput(const Input& input) override;

 private:
  static const inline QString kAltTitle{};
  static const inline QString kCommand{"'"};
  static const inline QString kDescription{
    "Search for files on your PC and open"};
  static const inline QString kIcon{
    Project::GetImageFilePath(defs::ImageFile::kFileSystemEntry)};
  static const inline QString kId{"397662aa-d686-48e3-b454-2d059cbd7ea0"};
  static const inline QString kPlaceholder{};
  static const inline QString kTitle{"Open file"};

  Finder finder_;
};

#endif  // FILESYSTEMENTRY_H
