#include "filesystementry.h"

#include <QStandardPaths>

FileSystemEntry::FileSystemEntry()
    : ProcessedResultBuilder{kId,       kIcon,        kTitle,
                             kAltTitle, kDescription, kCommand},
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
           .toStdString()}} {}

FileSystemEntry::FileSystemEntry(const std::filesystem::path& path)
    : ProcessedResultBuilder{kId,
                             kIcon,
                             QString::fromUtf8(path.filename().string()),
                             kAltTitle,
                             QString::fromUtf8(path.string()),
                             kCommand} {}

bool FileSystemEntry::ProcessInput(const Input& input) {
  auto paths = finder_.Search(input.GetCmd().toStdString());
  if (paths.size() == 0) {
    return false;
  }

  for (const auto& path : paths) {
    processed_results_.emplace_back(path);
  }

  return true;
}
