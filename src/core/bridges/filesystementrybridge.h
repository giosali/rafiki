#pragma once

#include <QStandardPaths>
#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "featurebridge.h"

class FileSystemEntryBridge final : public FeatureBridge {
 public:
  std::vector<FeatureObject*> ProcessInput(const FeatureModel* feature_model,
                                           const QString& input) override;

 private:
  class Finder {
   public:
    Finder() = default;
    explicit Finder(const std::string& location, size_t max_count,
                    const std::vector<std::filesystem::path> home_dirs);

    std::vector<std::filesystem::path> Search(const std::string& input);

   private:
    std::vector<std::filesystem::path> Iterate(
      const std::filesystem::path& path, const std::string& input);
    std::vector<std::filesystem::path> Reiterate(
      const std::filesystem::path& path, const std::string& input,
      std::vector<std::filesystem::path>& entries);

    size_t count_;
    std::set<std::string> home_dir_names_;
    std::vector<std::filesystem::path> home_dirs_;
    std::set<std::string> ignored_dirs_;
    std::string location_;
    size_t max_count_;
  };

  static const std::unordered_map<std::string, std::pair<QString, QString>>
    kMimeTypePairs;

  Finder finder_{
    QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
      .toStdString(),
    10,
    {QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
       .toStdString(),
     QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
       .toStdString(),
     QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
       .toStdString(),
     QStandardPaths::writableLocation(QStandardPaths::MusicLocation)
       .toStdString(),
     QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)
       .toStdString(),
     QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
       .toStdString()}};
};
