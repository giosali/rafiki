#include "filesystementry.h"

#include <QDesktopServices>
#include <QIcon>
#include <QStandardPaths>
#include <QUrl>
#include <memory>

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
  SetId(17);
  SetPixmap(Paths::GetPath(Paths::Image::kFileSystemEntry));
  SetTitle("Open file");
}

FileSystemEntry::FileSystemEntry(const std::filesystem::path& path)
    : is_entry_{true} {
  SetDescription(QString::fromUtf8(path.string()));
  SetId(Crypto::Djb2(path));
  SetTitle(QString::fromUtf8(path.filename().string()));

  // Exits early if the filesystem entry is a directory and sets icon to that
  // of a folder.
  if (std::filesystem::is_directory(path)) {
    auto icon = QIcon::fromTheme(kMimeTypePairs.at("").first);
    SetPixmap(icon);
    return;
  }

  // Uses a basic placeholder image of a file if the path doesn't contain a
  // file extension or if the file extension isn't in the MIME type map.
  auto extension = path.extension().string();
  if (extension.empty() || !kMimeTypePairs.contains(extension)) {
    SetPixmap(Paths::GetPath(Paths::Image::kFile));
    return;
  }

  auto pair = kMimeTypePairs.at(extension);
  auto icon = QIcon::fromTheme(pair.first);
  SetPixmap(!icon.isNull() ? icon : QIcon::fromTheme(pair.second));
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
    default:
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
    default:
      break;
  }
}

const std::unordered_map<std::string, std::pair<QString, QString>>
  FileSystemEntry::kMimeTypePairs{
    // <extension, [preferred MIME type, fallback MIME type]>
    // Audio
    {".flac", {"audio-x-flac", "audio-x-generic"}},
    {".mp3", {"audio-x-mpeg", "audio-x-generic"}},
    {".opus", {"audio-x-generic", "audio-x-generic"}},
    {".wav", {"audio-x-wav", "audio-x-generic"}},
    // Archive
    {".gz", {"application-x-gzip", "application-x-gzip"}},
    {".zip", {"application-x-zip", "application-x-zip"}},
    // Misc
    {"", {"folder", "folder"}},
    // Image
    {".jpg", {"image-x-generic", "image-x-generic"}},
    {".jpeg", {"image-x-generic", "image-x-generic"}},
    {".png", {"image-x-generic", "image-x-generic"}},
    {".svg", {"image-svg+xml", "image-x-generic"}},
    // Text
    {".c", {"text-x-c", "text-x-generic"}},
    {".cpp", {"text-x-cpp", "text-x-generic"}},
    {".css", {"text-css", "text-x-generic"}},
    {".hpp", {"text-x-c++hdr", "text-x-generic"}},
    {".md", {"text-markdown", "text-x-generic"}},
    {".py", {"text-x-python", "text-x-generic"}},
    {".rs", {"text-rust", "text-x-generic"}},
    {".rtf", {"text-richtext", "text-x-generic"}},
    {".sass", {"text-x-sass", "text-x-generic"}},
    {".scss", {"text-x-sass", "text-x-generic"}},
    {".ts", {"text-x-typescript", "text-x-generic"}},
    {".txt", {"text-x-generic", "text-x-generic"}},
    // Video
    {".mp4", {"video-x-generic", "video-x-generic"}},
    {".webm", {"video-x-generic", "video-x-generic"}},
  };
