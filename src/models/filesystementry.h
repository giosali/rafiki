#ifndef FILESYSTEMENTRY_H
#define FILESYSTEMENTRY_H

#include <QChar>
#include <QKeyCombination>
#include <QString>
#include <filesystem>
#include <string>
#include <vector>

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
  static const QString kAltDescription;
  static const QString kAltTitle;
  static const bool kAppendSpaceToCommand;
  static const QString kCommand;
  static const QString kDescription;
  static const QString kIcon;
  static const QString kId;
  static const QChar kInternalCommand;
  static const QString kTitle;
  static const QString kTitlePlaceholder;

  Finder finder_;
  bool is_entry_;
};

#endif  // FILESYSTEMENTRY_H
