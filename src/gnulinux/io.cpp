#include "io.h"

#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <set>

#include "../core/utils.h"

namespace gnulinux {
std::unordered_map<std::string, std::pair<uintmax_t, std::filesystem::path>>
Io::CreateIconMap() {
  auto map = std::unordered_map<std::string,
                                std::pair<uintmax_t, std::filesystem::path>>{};
  auto icon_dirs = FormatBaseDirs("icons");
  auto theme = GetIconTheme();
  for (const auto& icon_dir : icon_dirs) {
    auto target_dir = icon_dir / theme;
    if (!std::filesystem::exists(target_dir)) {
      target_dir = icon_dir / "hicolor";

      // Means that the target directory is set to /usr/share/pixmaps.
      if (!std::filesystem::exists(target_dir)) {
        target_dir = icon_dir;
      }
    }

    // Looks for the icons.
    for (const auto& entry : std::filesystem::recursive_directory_iterator(
           target_dir,
           std::filesystem::directory_options::skip_permission_denied)) {
      if (!entry.exists() || entry.is_directory()) {
        continue;
      }

      auto stem = entry.path().stem().string();
      if (auto it = map.find(stem); it != map.end()) {
        auto file_size = entry.file_size();
        if (it->second.first < file_size) {
          it->second = {file_size, entry};
        }

        continue;
      }

      map.insert({stem, {entry.file_size(), entry}});
    }
  }

  auto index_file = std::filesystem::path{};
  auto theme_dir_parent = std::filesystem::path{};
  for (const auto& icon_dir : icon_dirs) {
    auto theme_dir = icon_dir / theme;
    if (!std::filesystem::exists(theme_dir)) {
      continue;
    }

    auto index_file = theme_dir / "index.theme";
    theme_dir_parent = icon_dir;
    break;
  }

  auto index_settings =
    QSettings{QString::fromStdString(index_file), QSettings::IniFormat};

  // hicolor must be the fallback theme if `Inherits` is not found,
  auto inherits_val =
    index_settings.value("Icon Theme/Inherits", "hicolor").toString();
  auto inherits = inherits_val.split(',');
  for (const auto& inherited_theme : inherits) {
    auto theme_dir = theme_dir_parent / inherited_theme.toStdString();
    if (!std::filesystem::exists(theme_dir)) {
      continue;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(
           theme_dir,
           std::filesystem::directory_options::skip_permission_denied)) {
      if (entry.is_directory()) {
        continue;
      }

      // Only adds icons that aren't in the map.
      auto stem = entry.path().stem().string();
      if (auto it = map.find(stem); it == map.end()) {
        map.insert({stem, {entry.file_size(), entry}});
      }
    }
  }

  return map;
}

std::vector<std::filesystem::path> Io::FormatBaseDirs(
  const std::string& subdir) {
  auto home_path = std::filesystem::path{std::getenv("HOME")};
  auto is_icons_subdir = subdir == "icons";

  // The order of the base directories matters.
  // Using `std::set` instead of a `std::vector` won't work because it sorts
  // elements automatcially.
  // Using `std::unordered_set` won't work either because it also sorts
  // elements,
  auto base_dirs = std::vector<std::filesystem::path>{};

  // https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html
  if (is_icons_subdir) {
    base_dirs.push_back(home_path / ".icons");
  }

  // https://wiki.archlinux.org/title/desktop_entries#Application_entry
  base_dirs.push_back(home_path / ".local/share");
  base_dirs.push_back("/usr/local/share/" + subdir);
  base_dirs.push_back("/usr/share/" + subdir);

  // https://specifications.freedesktop.org/basedir-spec/basedir-spec-0.6.html
  auto xdg_data_dirs_val = std::getenv("XDG_DATA_DIRS");
  auto xdg_data_dirs = xdg_data_dirs_val == nullptr
                         ? "/usr/local/share/:/usr/share/"
                         : xdg_data_dirs_val;

  // Adds the data directories found in the XDG_DATA_DIRS environment variable.
  for (const auto& dir : utils::Split(xdg_data_dirs, ':')) {
    auto subdir_path = std::filesystem::path{dir} / subdir;
    // Skips the subdirectory if it's already present in the vector.
    if (std::find(base_dirs.begin(), base_dirs.end(), subdir_path) !=
        base_dirs.end()) {
      continue;
    }

    base_dirs.push_back(subdir_path);
  }

  // https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html
  // This directory must be added last.
  if (is_icons_subdir) {
    // Adds the directory if it isn't already in the vector.
    if (auto p = std::filesystem::path{"/usr/share/pixmaps/" + subdir};
        std::find(base_dirs.begin(), base_dirs.end(), p) == base_dirs.end()) {
      base_dirs.push_back("/usr/share/pixmaps");
    }
  }

  // Removes directories that don't exist.
  base_dirs.erase(
    std::remove_if(base_dirs.begin(), base_dirs.end(),
                   [](const auto& i) { return !std::filesystem::exists(i); }),
    base_dirs.end());
  return base_dirs;
}

std::filesystem::path Io::GetDesktopEntryIcon(const std::string& icon_value) {
  auto base_dirs = FormatBaseDirs("icons");

  // Pair consists of: <image_file, icon_base_dir>
  auto image_file_pairs =
    std::vector<std::pair<std::filesystem::path, std::filesystem::path>>{};

  for (const auto& base_dir : base_dirs) {
    if (!std::filesystem::exists(base_dir)) {
      continue;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(
           base_dir,
           std::filesystem::directory_options::skip_permission_denied)) {
      if (entry.is_directory() || entry.path().stem() != icon_value) {
        continue;
      }

      image_file_pairs.push_back({entry, base_dir});
    }
  }

  // User base directory will always be first. It's hard-coded that way.
  auto user_base_dir = base_dirs[0];

  auto icon = std::filesystem::path{};
  auto contains_user_base_dir = false;
  auto contains_icon_theme_base_dir = false;
  auto icon_theme = GetIconTheme();
  for (const auto& image_file_pair : image_file_pairs) {
    auto image_file = image_file_pair.first;

    // Checks if the path of the current image file contains the path of the
    // user base directory.
    if (!std::filesystem::relative(image_file, user_base_dir).empty()) {
      contains_user_base_dir = true;
    } else if (!icon.empty() && contains_user_base_dir) {
      // Returns the current icon if:
      // 1. the icon isn't empty.
      // 2. the path to the current image file does not contain the path of
      // the user base directory, implying that we are no longer iterating
      // through image files that were discovered in the user base directory.
      break;
    }

    // Check is the path of the current image file contains the user's icon
    // theme. This should be prioritized *only* after checking the user base
    // directory.
    if (!std::filesystem::relative(image_file,
                                   image_file_pair.second / icon_theme)
           .empty()) {
      contains_icon_theme_base_dir = true;
    } else if (!icon.empty() && contains_icon_theme_base_dir) {
      // Returns the current icon if:
      // 1. the icon isn't empty.
      // 2. the path to the current image file does not contain the icon
      // theme, implying that we are no longer iterating through image files
      // that were discovered in the icon theme directory.
      break;
    }

    if (icon.empty() || std::filesystem::file_size(image_file) >
                          std::filesystem::file_size(icon)) {
      icon = image_file;
    }
  }

  return icon;
}

std::string Io::GetIconTheme() {
  // Checks in the following order: GNOME, Cinnamon, Mate, Xfce.
  // https://stackoverflow.com/a/44629154
  // https://github.com/cerebroapp/cerebro/issues/17
  auto commands = std::array<QString, 4>{
    "gsettings get org.gnome.desktop.interface icon-theme",
    "gsettings get org.cinnamon.desktop.interface icon-theme",
    "gsettings get org.mate.interface icon-theme",
    "xfconf-query -lvc xsettings -p /Net/ThemeName"};

  auto theme = QString{};
  for (size_t i = 0; i < commands.size(); ++i) {
    if (auto result = Execute(commands[i]); !result.isEmpty()) {
      // Strips apostrophes from both sides since the theme is returned as
      // 'Theme' on GNOME.
      result = result.trimmed();
      qsizetype pos = result.front() == '\'' ? 1 : 0;
      qsizetype n = result.length() - pos - (result.back() == '\'' ? 1 : 0);
      theme = result.sliced(pos, n);
      break;
    }
  }

  return theme.toStdString();
}

std::vector<DesktopEntry> Io::ParseDesktopEntries() {
  auto icon_map = gnulinux::Io::CreateIconMap();
  auto desktop_entries = std::vector<DesktopEntry>{};
  auto desktop_entry_ids = std::set<std::filesystem::path>{};
  auto applications_dirs = gnulinux::Io::FormatBaseDirs("applications");
  for (const auto& applications_dir : applications_dirs) {
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(applications_dir)) {
      if (entry.is_directory()) {
        continue;
      }

      auto entry_path = entry.path();
      if (entry_path.extension() != ".desktop") {
        continue;
      }

      // Checks if the desktop file ID has already been encountered.
      auto rel_path = std::filesystem::relative(entry, applications_dir);
      if (desktop_entry_ids.find(rel_path) != desktop_entry_ids.end()) {
        continue;
      }

      desktop_entry_ids.insert(rel_path);

      auto desktop_entry = DesktopEntry(entry_path, icon_map);
      if (desktop_entry.NoDisplay()) {
        continue;
      }

      desktop_entries.push_back(desktop_entry);
    }
  }

  return desktop_entries;
}

QString Io::Execute(const QString& command) {
  auto parts = QProcess::splitCommand(command);
  auto process = QProcess{};
  process.start(parts.takeFirst(), parts);
  process.waitForFinished();
  return process.readAllStandardOutput();
}
}  // namespace gnulinux
