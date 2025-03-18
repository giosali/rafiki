#include "fetcher.h"

#include <QJsonArray>
#include <QStandardPaths>
#include <QtGlobal>
#include <algorithm>
#include <array>
#include <system_error>
#include <unordered_set>

#include "file.h"
#include "paths.h"

std::vector<std::filesystem::path> Fetcher::FetchDesktopEntryPaths() const {
  // https://wiki.archlinux.org/title/desktop_entries#Application_entry
  // The reason why a std::unordered_set isn't used instead of a std::vector is
  // because the order in which the locations are added matters.
  auto desktop_entry_locations = std::vector<std::filesystem::path>{};

  // From the documentation concerning the XDG Base Directory Specification:
  // `If $XDG_DATA_DIRS is either not set or empty, a value equal to
  // /usr/local/share/:/usr/share/ should be used.`
  // https://specifications.freedesktop.org/basedir-spec/basedir-spec-0.6.html
  //
  // The locations containing Flatpaks/Snaps/etc should typically be found via
  // this environment variable.
  auto xdg_data_dirs =
    qEnvironmentVariable("XDG_DATA_DIRS", "/usr/local/share/:/usr/share/");
  for (const auto& directory : xdg_data_dirs.split(':')) {
    auto subdirectory =
      std::filesystem::path{directory.toStdString()} / "applications";
    if (std::ranges::find(desktop_entry_locations, subdirectory) ==
        desktop_entry_locations.end()) {
      desktop_entry_locations.push_back(subdirectory);
    }
  }

  // `~/.local/share/applications/ for user-specific applications. User entries
  // take precedence over system entries.`
  if (auto local_location =
        std::filesystem::path{
          QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)
            .toStdString()};
      std::ranges::find(desktop_entry_locations, local_location) ==
      desktop_entry_locations.end()) {
    desktop_entry_locations.push_back(local_location);
  }

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
