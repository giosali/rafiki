#pragma once

#include <QString>

class Paths {
 public:
  enum class Directory { kAppConfig, kDataResources, kUserIcons };
  enum class Icon { kGitHub, kX };
  enum class Json { kUserSettings, kUserWebSearches, kWebSearches };

  Paths() = delete;

  static QString FormatIconPath(const QString& filename);
  static QString GetPath(Directory d);
  static QString GetPath(Json f);
};
