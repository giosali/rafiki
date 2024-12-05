#pragma once

#include <QSettings>
#include <QString>

class Paths {
 public:
  enum class Directory {
    kAppConfig,
    kDataResources,
    kImageResources,
    kUserIcons
  };
  enum class Image {
    kCalculator,
    kFile,
    kFileSystemEntry,
    kFolder,
    kQuestionMark,
    kRafiki,
    kSettings,
    kTrash,
    kUrl
  };
  enum class Json { kUserSettings, kUserWebSearches, kWebSearches };

  Paths() = delete;

  static QString GetPath(Directory d);
  static QString GetPath(Image f);
  static QString GetPath(Json f);
};
