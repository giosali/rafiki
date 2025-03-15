#include "fetcher.h"

#include <QJsonArray>
#include <QStandardPaths>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <string>
#include <system_error>
#include <unordered_set>

#include "file.h"
#include "paths.h"
#include "utilities.h"

std::vector<std::filesystem::path> Fetcher::FetchDesktopEntryPaths() const {
  // https://wiki.archlinux.org/title/desktop_entries#Application_entry
  // These are for applications installed system-wide.
  auto desktop_entry_locations = std::vector<std::filesystem::path>{
    "/usr/share/applications", "/usr/local/share/applications"};

  // https://specifications.freedesktop.org/basedir-spec/basedir-spec-0.6.html
  // These are typically for applications installed as Flatpaks/Snaps/etc.
  if (auto xdg_data_dirs = std::getenv("XDG_DATA_DIRS");
      xdg_data_dirs != nullptr) {
    for (const auto& directory : Utilities::Split(xdg_data_dirs, ':')) {
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
  auto paths = std::array<Paths::Json, 2>{Paths::Json::kWebSearches,
                                          Paths::Json::kUserWebSearches};
  for (const auto& path : paths) {
    if (auto document = File::Read(path); document.isArray()) {
      for (const auto& value : document.array()) {
        if (value.isObject()) {
          objects.push_back(value.toObject());
        }
      }
    }
  }

  return objects;
}
