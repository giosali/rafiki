#include "filesystementry.h"

#include <QDesktopServices>
#include <QStandardPaths>
#include <QUrl>
#include <memory>

#include "../core/config.h"
#include "../core/crypto.h"
#include "../core/paths.h"

FileSystemEntry::FileSystemEntry()
    : finder_{QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                .toStdString(),
              10,
              {QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                 .toStdString(),
               QStandardPaths::writableLocation(
                 QStandardPaths::DocumentsLocation)
                 .toStdString(),
               QStandardPaths::writableLocation(
                 QStandardPaths::DownloadLocation)
                 .toStdString(),
               QStandardPaths::writableLocation(QStandardPaths::MusicLocation)
                 .toStdString(),
               QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)
                 .toStdString(),
               QStandardPaths::writableLocation(
                 QStandardPaths::PicturesLocation)
                 .toStdString()}},
      is_entry_{false} {
  SetDescription("Search for files on your PC and open");
  SetIcon(Paths::Path(Paths::Image::kFileSystemEntry));
  SetId(Config::kApplicationAuthorId, 17);
  SetTitle("Open file");
}

FileSystemEntry::FileSystemEntry(const std::filesystem::path& path)
    : is_entry_{true} {
  SetDescription(QString::fromUtf8(path.string()));
  SetIcon(Paths::Icon(path));
  SetId(Config::kApplicationAuthorId, Crypto::Djb2(path));
  SetTitle(QString::fromUtf8(path.filename().string()));
}

bool FileSystemEntry::ProcessInput(const Input& input) {
  results_.clear();

  auto text = input.ToString();
  if (text[0] != '\'') {
    return false;
  }

  // Means that the current user input is a single command character.
  if (text.length() == 1) {
    return true;
  }

  auto paths = finder_.Search(text.sliced(1).toStdString());
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
    emit Dragged(GetDescription());
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
                                  GetDescription().toStdString()}
                                                         .parent_path()
                                                         .string())
                              : GetDescription());
      emit Hidden();
      QDesktopServices::openUrl(url);
      break;
    }
    case Qt::Key_Alt:
      emit NewDescriptionRequested("Show in folder");
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
      emit NewDescriptionRequested(GetDescription());
      break;
  }
}
