#ifndef FILESYSTEMENTRY_H
#define FILESYSTEMENTRY_H

#include <QKeyCombination>
#include <QString>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>

#include "../core/finder.h"
#include "../core/input.h"
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
  static const std::unordered_map<std::string, std::pair<QString, QString>>
    kMimeTypePairs;

  Finder finder_;
  bool is_entry_;
};

#endif  // FILESYSTEMENTRY_H
