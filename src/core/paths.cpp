#include "paths.h"

#include <QStandardPaths>
#include <QtSystemDetection>

#ifdef Q_OS_LINUX
#include "../gnulinux/io.h"
#endif

QString Paths::Icon(const std::filesystem::path& p) {
  // Only runs once when first invoked.
  static auto _ = []() {
#ifdef Q_OS_LINUX
    mimetype_icons_ = gnulinux::Io::GetMimeTypeIcons();
#endif
    return true;
  }();

  auto extension = p.extension().string();
  auto is_directory = std::filesystem::is_directory(p);

  // Exits with placeholder image if path doesn't contain a file extension and
  // isn't a directory.
  if (extension.empty() && !is_directory) {
    return Path(Image::kFile);
  }

  // Returns a generic resource image if there was no match.
  auto it = mimetype_icons_.find(extension);
  return it == mimetype_icons_.end()
           ? Path(is_directory ? Image::kFolder : Image::kFile)
           : it->second;
}

QString Paths::Path(Directory d) {
  auto path = QString{};
  switch (d) {
    case Directory::kAppConfig:
      path =
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
      break;
    case Directory::kAutostart:
      path = Combine(
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation),
        "autostart");
      break;
    case Directory::kDataResources:
      path = "://data";
      break;
    case Directory::kImageResources:
      path = "://images";
      break;
    case Directory::kYourIcons:
      path = Combine(Path(Directory::kAppConfig), "youricons");
      break;
  }

  return path;
}

QString Paths::Path(Ini f) {
  auto directory = QString{};
  auto filename = QString{};
  switch (f) {
    case Ini::kDefault:
      directory = Path(Directory::kDataResources);
      filename = "settings.ini";
      break;
    case Ini::kRafikiDesktopEntry:
      directory = Path(Directory::kDataResources);
      filename = "rafiki.desktop";
      break;
    case Ini::kUser:
      directory = Path(Directory::kAppConfig);
      filename = "settings.ini";
      break;
  }

  return Combine(directory, filename);
}

QString Paths::Path(Image f) {
  auto filename = QString{};
  switch (f) {
    case Image::kCalculator:
      filename = "calculator.png";
      break;
    case Image::kFile:
      filename = "file.svg";
      break;
    case Image::kFileSystemEntry:
      filename = "filesystementry.svg";
      break;
    case Image::kFolder:
      filename = "folder.svg";
      break;
    case Image::kQuestionMark:
      filename = "question-mark.png";
      break;
    case Image::kRafiki:
      filename = "rafiki.png";
      break;
    case Image::kTrash:
      filename = "trash.svg";
      break;
    case Image::kUrl:
      filename = "url.svg";
      break;
  }

  return filename.isEmpty()
           ? filename
           : Combine(Path(Directory::kImageResources), filename);
}

QString Paths::Path(Json f) {
  auto directory = QString{};
  auto filename = QString{};
  switch (f) {
    case Json::kWebSearches:
      directory = Path(Directory::kDataResources);
      filename = "web-searches.json";
      break;
    case Json::kYourWebSearches:
      directory = Path(Directory::kAppConfig);
      filename = "your-web-searches.json";
      break;
  }

  return Combine(directory, filename);
}

QString Paths::Combine(const QString& p1, const QString& p2) {
  return p1.isEmpty() || p2.isEmpty() ? QString{} : p1 + "/" + p2;
}

std::unordered_map<std::string, QString> Paths::mimetype_icons_{};
