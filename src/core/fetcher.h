#pragma once

#include <QJsonObject>
#include <filesystem>
#include <vector>

class Fetcher {
 public:
  Fetcher() = default;

  std::vector<std::filesystem::path> FetchDesktopEntryDirectoryPaths() const;
  std::vector<std::filesystem::path> FetchDesktopEntryPaths() const;
  std::vector<QJsonObject> FetchWebSearchObjects() const;
};
