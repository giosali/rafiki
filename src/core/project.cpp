#include "project.h"

#include <QCoreApplication>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUuid>
#include <QtSystemDetection>
#include <algorithm>
#include <set>

#ifdef Q_OS_LINUX
#include "../gnulinux/desktopentry.h"
#include "../gnulinux/io.h"
#endif
#include "../models/application.h"
#include "../models/calculator.h"
#include "../models/filesystementry.h"
#include "../models/trash.h"
#include "../models/websearch.h"
#include "utils.h"

std::vector<std::shared_ptr<BaseResult>> Project::FindBaseResults(
  const Input& input) {
  auto results_concat = std::vector<std::shared_ptr<BaseResult>>{};

  auto suggestions = autocomplete_.Find(input);
  for (const auto& suggestion : suggestions) {
    auto results_it = base_results_map_.find(suggestion);
    if (results_it == base_results_map_.end()) {
      continue;
    }

    auto results = results_it->second;
    results_concat.insert(results_concat.end(), results.begin(), results.end());
  }

  for (const auto& pbr : processed_base_results_) {
    if (!pbr->ProcessInput(input)) {
      continue;
    }

    results_concat.push_back(pbr);
  }

  for (const auto& prb : processed_result_builders_) {
    if (!prb->ProcessInput(input)) {
      continue;
    }

    auto results = prb->GetResults();
    if (results.empty()) {
      results_concat.push_back(prb);
    } else {
      results_concat.insert(results_concat.end(), results.begin(),
                            results.end());
    }
  }

  return results_concat;
}

QString Project::GetDataFilePath(defs::DataFile file) {
  auto dir = QString{"://data/"};
  switch (file) {
    case defs::DataFile::kSettings:
      return dir + "settings.ini";
    case defs::DataFile::kWebSearches:
      return dir + "web-searches.json";
    default:
      return QString{};
  }
}

std::vector<std::shared_ptr<BaseResult>> Project::GetDefaultBaseResults() {
  return default_base_results_;
}

QString Project::GetImageFilePath(defs::ImageFile file) {
  auto dir = QString{"://images/"};
  switch (file) {
    case defs::ImageFile::kCalculator:
      return dir + "calculator.png";
    case defs::ImageFile::kFile:
      return dir + "file.svg";
    case defs::ImageFile::kFileSystemEntry:
      return dir + "filesystementry.svg";
    case defs::ImageFile::kFolder:
      return dir + "folder.svg";
    case defs::ImageFile::kQuestionMark:
      return dir + "question-mark.png";
    case defs::ImageFile::kTrash:
      return dir + "trash.svg";
    default:
      return QString{};
  }
}

QString Project::GetMimeTypeImagePath(const std::filesystem::path& path) {
  auto extension = path.extension().string();

  // Exits if path doesn't contain a file extension and isn't a directory.
  if (extension.empty() && !std::filesystem::is_directory(path)) {
    return GetImageFilePath(defs::ImageFile::kFile);
  }

  // Returns a generic resource image if there was no match.
  auto search = mimetype_images_map_.find(extension);
  return search == mimetype_images_map_.end()
           ? GetImageFilePath(std::filesystem::is_directory(path)
                                ? defs::ImageFile::kFolder
                                : defs::ImageFile::kFile)
           : search->second;
}

void Project::Initialize() {
  // Sets up default settings.
  auto default_settings = GetUserSettings();
  auto user_settings = GetDefaultSettings();

  for (const auto& group : default_settings.childGroups()) {
    SetMissingSettings(default_settings, user_settings, group);
  }

  // Sets up applications.
#ifdef Q_OS_LINUX
  auto desktop_entries = gnulinux::Io::ParseDesktopEntries();
  for (const auto& desktop_entry : desktop_entries) {
    auto application = std::make_shared<Application>(
      desktop_entry.GetName(), desktop_entry.GetIcon(),
      desktop_entry.GetIconSize(), desktop_entry.GetDescription(),
      desktop_entry.GetExec());
    auto cmd = application->FormatCommand();
    autocomplete_.Insert(cmd);
    base_results_map_[cmd].push_back(application);
  }
#endif

  // Sets up search results based on data files.
  ParseJsonToBaseResults<WebSearch>(
    GetDataFilePath(defs::DataFile::kWebSearches));

  // Sets up built-in search results not based on data files.
  AddBaseResult(std::make_shared<Trash>());
  AddProcessedBaseResult(std::make_shared<Calculator>());
  AddProcessedResultBuilder(std::make_shared<FileSystemEntry>());

  // Sets up default search results.
  UpdateDefaultBaseResults();

#ifdef Q_OS_LINUX
  // Fetches icon theme on Linux (for GNOME) and adds MIME types.
  // For more info:
  // https://stackoverflow.com/a/44629154
  // https://github.com/cerebroapp/cerebro/issues/17
  if (auto theme = gnulinux::Io::GetIconTheme(); !theme.empty()) {
    AddMimeTypeImage(theme, ".mp3", "audio-x-mpeg", "audio-x-generic");
    AddMimeTypeImage(theme, ".flac", "audio-x-flac", "audio-x-generic");
    AddMimeTypeImage(theme, ".opus", "audio-x-generic", "audio-x-generic");
    AddMimeTypeImage(theme, ".wav", "audio-x-wav", "audio-x-generic");

    AddMimeTypeImage(theme, ".gz", "application-x-gzip", "application-x-gzip");

    AddMimeTypeImage(theme, ".zip", "application-x-zip", "application-x-zip");

    AddMimeTypeImage(theme, "", "folder", "folder");

    AddMimeTypeImage(theme, ".png", "image-x-generic", "image-x-generic");
    AddMimeTypeImage(theme, ".jpg", "image-x-generic", "image-x-generic");
    AddMimeTypeImage(theme, ".jpeg", "image-x-generic", "image-x-generic");
    AddMimeTypeImage(theme, ".svg", "image-svg+xml", "image-x-generic");

    AddMimeTypeImage(theme, ".txt", "text-x-generic", "text-x-generic");
    AddMimeTypeImage(theme, ".css", "text-css", "text-x-generic");
    AddMimeTypeImage(theme, ".md", "text-markdown", "text-x-generic");
    AddMimeTypeImage(theme, ".rtf", "text-richtext", "text-x-generic");
    AddMimeTypeImage(theme, ".rs", "text-rust", "text-x-generic");
    AddMimeTypeImage(theme, ".c", "text-x-c", "text-x-generic");
    AddMimeTypeImage(theme, ".hpp", "text-x-c++hdr", "text-x-generic");
    AddMimeTypeImage(theme, ".cpp", "text-x-cpp", "text-x-generic");
    AddMimeTypeImage(theme, ".py", "text-x-python", "text-x-generic");
    AddMimeTypeImage(theme, ".sass", "text-x-sass", "text-x-generic");
    AddMimeTypeImage(theme, ".scss", "text-x-sass", "text-x-generic");
    AddMimeTypeImage(theme, ".ts", "text-x-typescript", "text-x-generic");

    AddMimeTypeImage(theme, ".mp4", "video-x-generic", "video-x-generic");
    AddMimeTypeImage(theme, ".webm", "video-x-generic", "video-x-generic");
  }
#endif
}

void Project::AddBaseResult(const std::shared_ptr<BaseResult>& base_result) {
  if (!base_result->HasCommand()) {
    return;
  }

  auto cmd = base_result->FormatCommand();
  autocomplete_.Insert(cmd);
  base_results_map_[cmd].push_back(base_result);
}

void Project::AddMimeTypeImage(const std::string& theme,
                               const std::string& extension,
                               const std::string& mimetype,
                               const std::string& mimetype_fallback) {
  auto theme_dir = std::filesystem::path{"/usr/share/icons/" + theme};
  if (!std::filesystem::exists(theme_dir)) {
    return;
  }

  auto dirs = std::vector<std::filesystem::path>{};
  for (const auto& entry : std::filesystem::directory_iterator{theme_dir}) {
    auto filename = entry.path().filename().string();
    if (filename[0] == '.' || !entry.is_directory()) {
      continue;
    }

    dirs.push_back(entry);
  }

  // Sorts the child directories in reverse alphabetical order.
  std::sort(dirs.begin(), dirs.end(),
            [](const std::filesystem::path& p1,
               const std::filesystem::path& p2) -> bool {
              return utils::CompareStrings(p1.filename().string(),
                                           p2.filename().string(), true);
            });

  // Looks through child directories for an image file whose name matches the
  // specified mimetype.
  auto find_mimetype_image = [&dirs,
                              &extension](const std::string& mimetype) -> bool {
    for (const auto& dir : dirs) {
      for (const auto& entry :
           std::filesystem::recursive_directory_iterator(dir)) {
        if (entry.is_directory()) {
          continue;
        }

        auto entry_path = entry.path();
        auto stem = entry_path.stem().string();
        if (stem != mimetype) {
          continue;
        }

        mimetype_images_map_.insert(
          {extension, QString::fromStdString(entry_path.string())});
        return true;
      }
    }

    return false;
  };

  if (!find_mimetype_image(mimetype)) {
    find_mimetype_image(mimetype_fallback);
  }
}

void Project::AddProcessedBaseResult(
  const std::shared_ptr<ProcessedResult>& processed_result) {
  if (processed_result->HasCommand()) {
    auto cmd = processed_result->FormatCommand();
    autocomplete_.Insert(cmd);
  }

  processed_base_results_.push_back(processed_result);
}

void Project::AddProcessedResultBuilder(
  const std::shared_ptr<ProcessedResultBuilder>& builder) {
  processed_result_builders_.push_back(builder);
}

QSettings Project::GetDefaultSettings() {
  return QSettings{GetDataFilePath(defs::DataFile::kSettings),
                   QSettings::IniFormat};
}

QSettings Project::GetUserSettings() {
  return QSettings{QSettings::IniFormat, QSettings::UserScope,
                   QCoreApplication::organizationName(),
                   QCoreApplication::applicationName()};
}

template <typename T>
void Project::ParseJsonToBaseResults(const QString& path) {
  auto file = QFile{path};
  if (!file.exists()) {
    return;
  }

  file.open(QIODevice::ReadOnly | QIODevice::Text);
  auto doc = QJsonDocument::fromJson(file.readAll());

  auto arr = doc.array();
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    auto result = std::make_shared<T>(it->toObject());
    auto cmd = result->FormatCommand();
    autocomplete_.Insert(cmd);
    base_results_map_[cmd].push_back(result);
  }
}

/// @brief Writes default settings to the user's filesystem.
/// @param internal_settings
/// @param external_settings
/// @param group The name of the INI section.
void Project::SetMissingSettings(QSettings& internal_settings,
                                 QSettings& external_settings,
                                 const QString& group) {
  internal_settings.beginGroup(group);
  external_settings.beginGroup(group);

  for (const auto& key : internal_settings.allKeys()) {
    if (external_settings.contains(key)) {
      continue;
    }

    external_settings.setValue(key, internal_settings.value(key));
  }

  external_settings.endGroup();
  internal_settings.endGroup();
}

void Project::UpdateDefaultBaseResults() {
  default_base_results_.clear();
  auto user_settings = GetUserSettings();

  // Takes the IDs of the default search results and stores them in a set.
  user_settings.beginGroup("DefaultSearchResults");

  auto guid_set = std::set<QUuid>();
  for (const auto& key : user_settings.allKeys()) {
    guid_set.insert(QUuid::fromString(user_settings.value(key).toString()));
  }

  user_settings.endGroup();

  for (const auto& [_, results] : base_results_map_) {
    for (auto result : results) {
      // TODO: update syntax in C++20.
      if (guid_set.find(result->GetId()) == guid_set.end()) {
        continue;
      }

      default_base_results_.push_back(result);
    }
  }
}
