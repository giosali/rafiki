#include "paths.h"

#include <QStandardPaths>
#include <QtSystemDetection>

QString Paths::GetPath(Directory d) {
  auto path = QString{};
  switch (d) {
    case Directory::kAppConfig:
      path =
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
      break;
    case Directory::kDataResources:
      path = "://data";
      break;
    case Directory::kImageResources:
      path = "://images";
      break;
    case Directory::kUserIcons:
      path = Combine(GetPath(Directory::kAppConfig), "icons");
      break;
  }

  return path;
}

QString Paths::GetPath(Image f) {
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
           : Combine(GetPath(Directory::kImageResources), filename);
}

QString Paths::GetPath(Json f) {
  auto directory = QString{};
  auto filename = QString{};
  switch (f) {
    case Json::kUserSettings:
      directory = GetPath(Directory::kAppConfig);
      filename = "settings.json";
      break;
    case Json::kUserWebSearches:
      directory = GetPath(Directory::kAppConfig);
      filename = "your-web-searches.json";
      break;
    case Json::kWebSearches:
      directory = GetPath(Directory::kDataResources);
      filename = "web-searches.json";
      break;
  }

  return Combine(directory, filename);
}

QString Paths::Combine(const QString& p1, const QString& p2) {
  return p1.isEmpty() || p2.isEmpty() ? QString{} : p1 + "/" + p2;
}

std::unordered_map<std::string, QString> Paths::mimetype_icons_{};
