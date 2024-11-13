#ifndef PATH_H
#define PATH_H

#include <QSettings>
#include <QString>
#include <string>
#include <unordered_map>

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
    kTrash,
    kUrl
  };
  enum class Json { kUserSettings, kUserWebSearches, kWebSearches };

  Paths() = delete;

  static QString GetPath(Directory d);
  static QString GetPath(Image f);
  static QString GetPath(Json f);

 private:
  static QString Combine(const QString& p1, const QString& p2);

  static std::unordered_map<std::string, QString> mimetype_icons_;
};

#endif  // PATH_H
