#include "desktopentry.h"

#include <QFile>

#include "../core/inifile.h"
#include "io.h"

namespace gnulinux {
DesktopEntry::DesktopEntry(
  const std::filesystem::path& desktop_entry_path,
  const std::unordered_map<
    std::string, std::pair<uintmax_t, std::filesystem::path>>& icon_map)
    : no_display_{false} {
  auto file = IniFile(desktop_entry_path);
  file.BeginSection("Desktop Entry");

  if (std::any_cast<bool>(file.GetValue<bool>("NoDisplay", false))) {
    no_display_ = true;
    return;
  }

  exec_ = QString::fromStdString(
    std::any_cast<std::string>(file.GetValue<std::string>("Exec")));
  name_ = QString::fromStdString(
    std::any_cast<std::string>(file.GetValue<std::string>("Name")));

  auto icon = std::any_cast<std::string>(file.GetValue<std::string>("Icon"));
  if (std::filesystem::exists(icon)) {
    icon_ = QString::fromStdString(icon);
  } else if (auto it = icon_map.find(icon); it != icon_map.end()) {
    icon_ = QString::fromStdString(it->second.second);
  }

  description_ = Io::GetExecutablePath(exec_);
}

QString DesktopEntry::GetDescription() const { return description_; }

QString DesktopEntry::GetExec() const { return exec_; }

QString DesktopEntry::GetIcon() const { return icon_; }

QString DesktopEntry::GetName() const { return name_; }

bool DesktopEntry::NoDisplay() const { return no_display_; }
}  // namespace gnulinux
