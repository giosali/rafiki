#ifndef IO_H
#define IO_H

#include <QString>
#include <QtSystemDetection>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "desktopentry.h"

#ifdef Q_OS_LINUX

namespace gnulinux {
class Io {
 public:
  Io() = delete;

  static std::unordered_map<std::string,
                            std::pair<size_t, std::filesystem::path>>
  CreateIconMap();
  static std::vector<std::filesystem::path> FormatBaseDirs(
    const std::string& subdir);
  static std::filesystem::path GetDesktopEntryIcon(const std::string& icon_val);
  static std::string GetIconTheme();
  static std::unordered_map<std::string, QString> GetMimeTypeIcons();
  static std::vector<DesktopEntry> ParseDesktopEntries();

 private:
  static QString Execute(const QString& command);
};
}  // namespace gnulinux

#endif  // Q_OS_LINUX
#endif  // IO_H
