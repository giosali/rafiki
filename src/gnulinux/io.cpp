#include "io.h"

#include <QProcess>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <set>

#include "../core/inifile.h"
#include "../core/utils.h"

namespace gnulinux {
std::unordered_map<std::string, QString> Io::GetMimeTypeIcons() {
  auto mimetype_extensions = std::unordered_map<std::string, QString>{};

  auto theme = GetIconTheme();
  if (theme.empty()) {
    return mimetype_extensions;
  }

  // Must check if theme directory exists since `theme` could be garbage.
  auto theme_path = std::filesystem::path{"/usr/share/icons"} / theme;
  if (!std::filesystem::exists(theme_path)) {
    return mimetype_extensions;
  }

  // "index.theme" MUST exist.
  auto theme_index = IniFile{theme_path / "index.theme"};

  // "Directories" is a required value. All lists in index.theme are
  // comma-separated.
  auto directories = theme_index.GetValue("Icon Theme/Directories").split(',');

  auto current_size = 0;
  auto mimetypes_directory = std::string{};
  for (const auto& directory : directories) {
    theme_index.BeginSection(directory);

    // "Context" is a required value.
    // Searches for the "Context" that is set to "MimeTypes".
    if (auto context = theme_index.GetValue("Context");
        context != "MimeTypes") {
      theme_index.EndSection();
      continue;
    }

    // "Size" is a required value.
    if (auto size = theme_index.GetValue("Size").toInt(); size > current_size) {
      current_size = size;
      mimetypes_directory = directory.toStdString();
    }

    theme_index.EndSection();
  }

  // <filename without extension, file path>
  auto mimetype_stems =
    std::unordered_map<std::string, std::filesystem::path>{};
  for (const auto& entry : std::filesystem::recursive_directory_iterator{
         theme_path / mimetypes_directory,
         std::filesystem::directory_options::skip_permission_denied}) {
    if (entry.is_directory()) {
      continue;
    }

    auto path = entry.path();
    if (auto extension = path.extension();
        extension == ".png" || extension == ".svg") {
      mimetype_stems.insert({path.stem().string(), path});
    }
  }

  // <extension, [preferred MIME type, fallback MIME type]>
  auto mimetype_pairs =
    std::array<std::pair<std::string, std::array<std::string, 2>>, 25>{
      {
        // Audio
        {".flac", {{"audio-x-flac", "audio-x-generic"}}},
        {".mp3", {{"audio-x-mpeg", "audio-x-generic"}}},
        {".opus", {{"audio-x-generic", "audio-x-generic"}}},
        {".wav", {{"audio-x-wav", "audio-x-generic"}}},
        // Archive
        {".gz", {{"application-x-gzip", "application-x-gzip"}}},
        {".zip", {{"application-x-zip", "application-x-zip"}}},
        // Misc
        {"", {{"folder", "folder"}}},
        // Image
        {".jpg", {{"image-x-generic", "image-x-generic"}}},
        {".jpeg", {{"image-x-generic", "image-x-generic"}}},
        {".png", {{"image-x-generic", "image-x-generic"}}},
        {".svg", {{"image-svg+xml", "image-x-generic"}}},
        // Text
        {".c", {{"text-x-c", "text-x-generic"}}},
        {".cpp", {{"text-x-cpp", "text-x-generic"}}},
        {".css", {{"text-css", "text-x-generic"}}},
        {".hpp", {{"text-x-c++hdr", "text-x-generic"}}},
        {".md", {{"text-markdown", "text-x-generic"}}},
        {".py", {{"text-x-python", "text-x-generic"}}},
        {".rs", {{"text-rust", "text-x-generic"}}},
        {".rtf", {{"text-richtext", "text-x-generic"}}},
        {".sass", {{"text-x-sass", "text-x-generic"}}},
        {".scss", {{"text-x-sass", "text-x-generic"}}},
        {".ts", {{"text-x-typescript", "text-x-generic"}}},
        {".txt", {{"text-x-generic", "text-x-generic"}}},
        // Video
        {".mp4", {{"video-x-generic", "video-x-generic"}}},
        {".webm", {{"video-x-generic", "video-x-generic"}}},
      },
    };

  for (const auto& pair : mimetype_pairs) {
    auto mimetypes = pair.second;

    // Searches for the preferred MIME type (and fallback MIME type if
    // necessary).
    for (const auto& mimetype : mimetypes) {
      if (auto it = mimetype_stems.find(mimetype); it != mimetype_stems.end()) {
        auto extension = pair.first;
        auto path = it->second;
        mimetype_extensions.insert({extension, QString::fromStdString(path)});
      }
    }
  }

  return mimetype_extensions;
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

QString Io::Execute(const QString& command) {
  auto parts = QProcess::splitCommand(command);
  auto process = QProcess{};
  process.start(parts.takeFirst(), parts);
  process.waitForFinished();
  return process.readAllStandardOutput();
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
}  // namespace gnulinux
