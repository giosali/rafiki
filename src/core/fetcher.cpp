#include "fetcher.h"

#include <QJsonArray>
#include <QStandardPaths>
#include <QString>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <system_error>
#include <unordered_set>
#include <utility>

#include "file.h"
#include "paths.h"
#include "utils.h"

std::vector<std::filesystem::path> Fetcher::FetchApplicationIconPaths() const {
  auto map = std::unordered_map<std::string,
                                std::pair<uintmax_t, std::filesystem::path>>{};
  // https://wiki.archlinux.org/title/desktop_entries#Application_entry
  // These are for applications installed system-wide.
  auto icon_locations = std::vector<std::filesystem::path>{
    "/usr/share/icons", "/usr/local/share/icons"};

  // This is only for checking uniqueness and remembering order since
  // unordered_sets don't keep track of order.
  auto icon_map = std::unordered_set<std::filesystem::path>{
    icon_locations.begin(), icon_locations.end()};

  // https://specifications.freedesktop.org/basedir-spec/basedir-spec-0.6.html
  // These are typically for applications installed as Flatpaks/Snaps/etc.
  if (auto xdg_data_dirs = std::getenv("XDG_DATA_DIRS");
      xdg_data_dirs != nullptr) {
    for (const auto& directory : utils::Split(xdg_data_dirs, ':')) {
      auto subdirectory = std::filesystem::path{directory} / "icons";
      if (!icon_map.contains(subdirectory)) {
        icon_locations.push_back(subdirectory);
        icon_map.insert(subdirectory);
      }
    }
  }

  // These directories are specific to icons. The final directory must be added
  // last.
  // https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html
  auto special_icon_locations = std::vector<std::filesystem::path>{
    std::filesystem::path{
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
        .toStdString()} /
      ".icons",
    "/usr/share/pixmaps"};
  for (const auto& location : special_icon_locations) {
    if (!icon_map.contains(location)) {
      icon_locations.push_back(location);
      icon_map.insert(location);
    }
  }

  return icon_locations;
}

std::vector<std::filesystem::path> Fetcher::FetchDesktopEntryPaths() const {
  // https://wiki.archlinux.org/title/desktop_entries#Application_entry
  // These are for applications installed system-wide.
  auto desktop_entry_locations = std::vector<std::filesystem::path>{
    "/usr/share/applications", "/usr/local/share/applications"};

  // https://specifications.freedesktop.org/basedir-spec/basedir-spec-0.6.html
  // These are typically for applications installed as Flatpaks/Snaps/etc.
  if (auto xdg_data_dirs = std::getenv("XDG_DATA_DIRS");
      xdg_data_dirs != nullptr) {
    for (const auto& directory : utils::Split(xdg_data_dirs, ':')) {
      auto subdirectory = std::filesystem::path{directory} / "applications";
      if (std::find(desktop_entry_locations.begin(),
                    desktop_entry_locations.end(),
                    subdirectory) == desktop_entry_locations.end()) {
        desktop_entry_locations.push_back(subdirectory);
      }
    }
  }

  // "~/.local/share/applications/ for user-specific applications. User entries
  // take precedence over system entries."
  desktop_entry_locations.push_back(
    QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)
      .toStdString());

  auto desktop_entries = std::vector<std::filesystem::path>{};
  auto desktop_entry_ids = std::unordered_set<std::filesystem::path>{};
  auto ec = std::error_code{};
  for (const auto& location : desktop_entry_locations) {
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator{location, ec}) {
      if (entry.is_directory()) {
        continue;
      }

      auto entry_path = entry.path();
      if (entry_path.extension() != ".desktop") {
        continue;
      }

      // Checks if the desktop file ID has already been encountered.
      auto desktop_entry_id = std::filesystem::relative(entry, location, ec);
      if (desktop_entry_ids.find(desktop_entry_id) != desktop_entry_ids.end()) {
        continue;
      }

      desktop_entry_ids.insert(desktop_entry_id);
      desktop_entries.push_back(entry);
    }
  }

  return desktop_entries;
}

std::vector<QJsonObject> Fetcher::FetchWebSearchObjects() const {
  auto objects = std::vector<QJsonObject>{};
  auto document = File::Read(Paths::Json::kWebSearches);
  if (!document.isArray()) {
    return {};
  }

  for (const auto& value : document.array()) {
    if (value.isObject()) {
      objects.push_back(value.toObject());
    }
  }

  return objects;
}
