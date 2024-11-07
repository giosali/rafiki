#ifndef FETCHER_H
#define FETCHER_H

#include <QJsonObject>
#include <filesystem>
#include <vector>

class Fetcher {
 public:
  Fetcher() = default;

  std::vector<std::filesystem::path> FetchDesktopEntryPaths() const;
  std::vector<QJsonObject> FetchWebSearchObjects() const;

 private:
};

#endif  // FETCHER_H
