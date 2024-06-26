#include "desktopentry.h"

#include "../core/inifile.h"
#include "../core/utils.h"

namespace gnulinux {
DesktopEntry::DesktopEntry(
  const std::filesystem::path& path,
  const std::unordered_map<
    std::string, std::pair<uintmax_t, std::filesystem::path>>& icon_map)
    : no_display_{false}, path_{QString::fromStdString(path)} {
  auto file = IniFile(path);
  file.BeginSection("Desktop Entry");

  if (std::any_cast<bool>(file.GetValue<bool>("NoDisplay", false))) {
    no_display_ = true;
    return;
  }

  exec_ = RemoveFieldCodes(
    std::any_cast<std::string>(file.GetValue<std::string>("Exec")));
  name_ = QString::fromStdString(
    std::any_cast<std::string>(file.GetValue<std::string>("Name")));

  auto icon = std::any_cast<std::string>(file.GetValue<std::string>("Icon"));
  if (std::filesystem::exists(icon)) {
    icon_ = QString::fromStdString(icon);
  } else if (auto it = icon_map.find(icon); it != icon_map.end()) {
    icon_ = QString::fromStdString(it->second.second);
  }
}

QString DesktopEntry::GetExec() const { return exec_; }

QString DesktopEntry::GetIcon() const { return icon_; }

uintmax_t DesktopEntry::GetIconSize() const {
  auto icon = std::filesystem::path{icon_.toStdString()};
  return std::filesystem::exists(icon) ? std::filesystem::file_size(icon) : 0;
}

QString DesktopEntry::GetName() const { return name_; }

QString DesktopEntry::GetPath() const { return path_; }

bool DesktopEntry::NoDisplay() const { return no_display_; }

QString DesktopEntry::RemoveFieldCodes(const std::string& exec) const {
  auto parts = utils::Split(exec);
  auto filtered_parts = std::vector<std::string>{};
  for (const auto& part : parts) {
    // https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html#exec-variables
    if (part.length() == 2 && part[0] == '%') {
      switch (part[1]) {
        case 'f':
        case 'F':
        case 'u':
        case 'U':
        case 'd':
        case 'D':
        case 'n':
        case 'N':
        case 'i':
        case 'c':
        case 'k':
        case 'v':
        case 'm':
          continue;
      }
    }

    filtered_parts.push_back(part);
  }

  switch (exec[0]) {
    case 'a':
      break;
  }

  return QString::fromStdString(utils::Join(filtered_parts, " "));
}
}  // namespace gnulinux
