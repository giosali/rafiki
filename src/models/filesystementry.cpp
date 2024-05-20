#include "filesystementry.h"

#include <QDesktopServices>
#include <QStandardPaths>
#include <QUrl>
#include <memory>

#include "../ui/mainwindow.h"

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
                             Project::GetMimeTypeImagePath(path),
                             QString::fromUtf8(path.filename().string()),
                             kTitlePlaceholder,
                             kAltTitle,
                             QString::fromUtf8(path.string()),
                             kCommand,
                             kAppendSpaceToCommand},
      is_entry_{true} {}

QString FileSystemEntry::DragAndDrop() {
  return is_entry_ ? description_ : QString{};
}

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

void FileSystemEntry::ProcessKeyPress(const QKeyCombination& combination) {
  if (!is_entry_) {
    return;
  }

  auto main_window = MainWindow::Get();
  if (main_window == nullptr) {
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
      main_window->Hide();
      QDesktopServices::openUrl(url);
      break;
    }
    case Qt::Key_Alt:
      main_window->SetSearchResultDescription(kAltDescription);
      break;
  }
}

void FileSystemEntry::ProcessKeyRelease(const QKeyCombination& combination) {
  if (!is_entry_) {
    return;
  }

  auto main_window = MainWindow::Get();
  if (main_window == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Alt:
      main_window->SetSearchResultDescription(description_);
      break;
  }
}
