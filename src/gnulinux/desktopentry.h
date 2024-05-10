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
    const std::filesystem::path& desktop_entry_path,
    const std::unordered_map<
      std::string, std::pair<uintmax_t, std::filesystem::path>>& icon_map);

  QString GetDescription() const;
  QString GetExec() const;
  QString GetIcon() const;
  QString GetName() const;
  bool NoDisplay() const;

 private:
  QString RemoveFieldCodes(const std::string& exec) const;

  bool no_display_;
  QString description_;
  QString exec_;
  QString icon_;
  QString name_;
};
}  // namespace gnulinux

#endif  // DESKTOPENTRY_H
