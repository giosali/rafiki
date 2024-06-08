#include "filesystementry.h"

#include <QDesktopServices>
#include <QStandardPaths>
#include <QUrl>
#include <memory>

#include "../core/io.h"

FileSystemEntry::FileSystemEntry()
    : ProcessedResultBuilder{kId,       kIcon,
                             kTitle,    kTitlePlaceholder,
                             kAltTitle, kDescription,
                             kCommand,  kAppendSpaceToCommand},
      finder_{
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
          .toStdString(),
        10,
        {QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
           .toStdString(),
         QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
           .toStdString(),
         QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
           .toStdString(),
         QStandardPaths::writableLocation(QStandardPaths::MusicLocation)
           .toStdString(),
         QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)
           .toStdString(),
         QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
           .toStdString()}},
      is_entry_{false} {}

FileSystemEntry::FileSystemEntry(const std::filesystem::path& path)
    : ProcessedResultBuilder{kId,
                             Io::GetIcon(path),
                             QString::fromUtf8(path.filename().string()),
                             kTitlePlaceholder,
                             kAltTitle,
                             QString::fromUtf8(path.string()),
                             kCommand,
                             kAppendSpaceToCommand},
      is_entry_{true} {}

bool FileSystemEntry::ProcessInput(const Input& input) {
  results_.clear();

  auto full = input.GetFull();
  if (full[0] != kInternalCommand) {
    return false;
  }

  // Means that the current user input is a single command character.
  if (full.length() == 1) {
    return true;
  }

  auto paths = finder_.Search(full.sliced(1).toStdString());
  if (paths.size() == 0) {
    return false;
  }

  for (const auto& path : paths) {
    results_.push_back(std::make_shared<FileSystemEntry>(path));
  }

  return true;
}

void FileSystemEntry::Drag() {
  if (is_entry_) {
    emit Dragged(description_);
  }
}

void FileSystemEntry::ProcessKeyPress(const QKeyCombination& combination,
                                      const Input& input) {
  if (!is_entry_) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Return: {
      // Opens either the containing directory of the entry or the entry itself.
      auto url =
        QUrl::fromLocalFile(combination.keyboardModifiers() & Qt::AltModifier
                              ? QString::fromStdString(std::filesystem::path{
                                  description_.toStdString()}
                                                         .parent_path()
                                                         .string())
                              : description_);
      emit Hidden();
      QDesktopServices::openUrl(url);
      break;
    }
    case Qt::Key_Alt:
      emit NewDescriptionRequested(kAltDescription);
      break;
  }
}

void FileSystemEntry::ProcessKeyRelease(const QKeyCombination& combination,
                                        const Input& input) {
  if (!is_entry_) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewDescriptionRequested(description_);
      break;
  }
}

const QString FileSystemEntry::kAltDescription{"Show in folder"};

const QString FileSystemEntry::kAltTitle{};

const bool FileSystemEntry::kAppendSpaceToCommand{false};

const QString FileSystemEntry::kCommand{};

const QString FileSystemEntry::kDescription{
  "Search for files on your PC and open"};

const QString FileSystemEntry::kIcon{
  Io::GetIcon(Io::ImageFile::kFileSystemEntry)};

const QString FileSystemEntry::kId{"397662aa-d686-48e3-b454-2d059cbd7ea0"};

const QChar FileSystemEntry::kInternalCommand{'\''};

const QString FileSystemEntry::kTitle{"Open file"};

const QString FileSystemEntry::kTitlePlaceholder{};
