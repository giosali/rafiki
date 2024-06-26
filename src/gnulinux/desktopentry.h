#ifndef DESKTOPENTRY_H
#define DESKTOPENTRY_H

#include <QString>
#include <filesystem>
#include <string>
#include <utility>

namespace gnulinux {
class DesktopEntry {
 public:
  explicit DesktopEntry(
    const std::filesystem::path& path,
    const std::unordered_map<
      std::string, std::pair<uintmax_t, std::filesystem::path>>& icon_map);

  QString GetExec() const;
  QString GetIcon() const;
  uintmax_t GetIconSize() const;
  QString GetName() const;
  QString GetPath() const;
  bool NoDisplay() const;

 private:
  QString RemoveFieldCodes(const std::string& exec) const;

  bool no_display_;
  QString exec_;
  QString icon_;
  QString name_;
  QString path_;
};
}  // namespace gnulinux

#endif  // DESKTOPENTRY_H
