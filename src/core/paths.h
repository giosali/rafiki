#pragma once

#include <QSettings>
#include <QString>

class Paths {
 public:
  enum class Directory { kAppConfig, kDataResources, kUserIcons };
  enum class Json { kUserSettings, kUserWebSearches, kWebSearches };

  Paths() = delete;

  static QString GetPath(Directory d);
  static QString GetPath(Json f);
};
