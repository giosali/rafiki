#ifndef PATH_H
#define PATH_H

#include <QSettings>
#include <QString>
#include <filesystem>
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

  static QString Icon(const std::filesystem::path& p);
  static QString Path(Directory d);
  static QString Path(Image f);
  static QString Path(Json f);

 private:
  static QString Combine(const QString& p1, const QString& p2);

  static std::unordered_map<std::string, QString> mimetype_icons_;
};

#endif  // PATH_H
