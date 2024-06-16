#include "io.h"

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
#include "../models/url.h"
#include "../models/websearch.h"
#include "utils.h"

std::vector<std::shared_ptr<BaseResult>> Io::FindBaseResults(
  const Input& input) {
  auto results_concat = std::vector<std::shared_ptr<BaseResult>>{};

  // Regular commands.
  auto suggestions = autocompleter_.Find(input);
  for (const auto& suggestion : suggestions) {
    auto results_it = base_results_map_.find(suggestion);
    if (results_it == base_results_map_.end()) {
      continue;
    }

    auto results = results_it->second;
    results_concat.insert(results_concat.end(), results.begin(), results.end());
  }

  // Input-dependent results.
  for (const auto& pbr : processed_base_results_) {
    if (!pbr->ProcessInput(input)) {
      continue;
    }

    results_concat.push_back(pbr);
  }

  // Input-dependent results that also produce results.
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

std::vector<std::shared_ptr<BaseResult>> Io::GetDefaultBaseResults() {
  return default_base_results_;
}

QString Io::GetIcon(ImageFile file) {
  auto dir = QString{"://images/"};
  switch (file) {
    case ImageFile::kCalculator:
      return dir + "calculator.png";
    case ImageFile::kFile:
      return dir + "file.svg";
    case ImageFile::kFileSystemEntry:
      return dir + "filesystementry.svg";
    case ImageFile::kFolder:
      return dir + "folder.svg";
    case ImageFile::kQuestionMark:
      return dir + "question-mark.png";
    case ImageFile::kTrash:
      return dir + "trash.svg";
    case ImageFile::kUrl:
      return dir + "url.svg";
    default:
      return QString{};
  }
}

QString Io::GetIcon(const std::filesystem::path& path) {
  auto extension = path.extension().string();

  // Exits if path doesn't contain a file extension and isn't a directory.
  if (extension.empty() && !std::filesystem::is_directory(path)) {
    return GetIcon(ImageFile::kFile);
  }

  // Returns a generic resource image if there was no match.
  auto search = mimetype_icons_.find(extension);
  return search == mimetype_icons_.end()
           ? GetIcon(std::filesystem::is_directory(path) ? ImageFile::kFolder
                                                         : ImageFile::kFile)
           : search->second;
}

void Io::Initialize() {
  // Sets up default settings.
  auto default_settings = GetFile(ConfigFile::kDefault);
  auto user_settings = GetFile(ConfigFile::kUser);

  // Adds missing/new settings from default settings to user settings (written
  // to disk).
  for (const auto& group : default_settings.childGroups()) {
    default_settings.beginGroup(group);
    user_settings.beginGroup(group);

    for (const auto& key : default_settings.childKeys()) {
      if (!user_settings.contains(key)) {
        user_settings.setValue(key, default_settings.value(key));
      }
    }

    default_settings.endGroup();
    user_settings.endGroup();
  }

#ifdef Q_OS_LINUX
  // Sets up applications.
  auto desktop_entries = gnulinux::Io::ParseDesktopEntries();
  for (const auto& desktop_entry : desktop_entries) {
    auto application = std::make_shared<Application>(
      desktop_entry.GetName(), desktop_entry.GetIcon(),
      desktop_entry.GetIconSize(), desktop_entry.GetDescription(),
      desktop_entry.GetExec());
    auto cmd = application->FormatCommand();
    autocompleter_.Insert(cmd);
    base_results_map_[cmd].push_back(application);
  }

  // Fetches icon theme on Linux (for GNOME) and adds MIME types.
  // For more info:
  // https://stackoverflow.com/a/44629154
  // https://github.com/cerebroapp/cerebro/issues/17
  mimetype_icons_ = gnulinux::Io::GetMimeTypeIcons();
#endif

  // Sets up search results based on data files.
  ParseJsonToBaseResults<WebSearch>(GetFile(DataFile::kWebSearches));

  // Sets up built-in search results not based on data files.
  AddBaseResult(std::make_shared<Trash>());
  AddProcessedBaseResult(std::make_shared<Calculator>());
  AddProcessedBaseResult(std::make_shared<Url>());
  AddProcessedResultBuilder(std::make_shared<FileSystemEntry>());

  // Sets up default search results.
  UpdateDefaultBaseResults();
}

void Io::AddBaseResult(const std::shared_ptr<BaseResult>& base_result) {
  if (!base_result->HasCommand()) {
    return;
  }

  auto cmd = base_result->FormatCommand();
  autocompleter_.Insert(cmd);
  base_results_map_[cmd].push_back(base_result);
}

void Io::AddProcessedBaseResult(
  const std::shared_ptr<ProcessedResult>& processed_result) {
  if (processed_result->HasCommand()) {
    auto cmd = processed_result->FormatCommand();
    autocompleter_.Insert(cmd);
  }

  processed_base_results_.push_back(processed_result);
}

void Io::AddProcessedResultBuilder(
  const std::shared_ptr<ProcessedResultBuilder>& builder) {
  processed_result_builders_.push_back(builder);
}

QSettings Io::GetFile(ConfigFile file) {
  switch (file) {
    case ConfigFile::kDefault:
      return QSettings{GetFile(DataFile::kSettings), QSettings::IniFormat};
    case ConfigFile::kUser:
      return QSettings{QSettings::IniFormat, QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName()};
      break;
    default:
      return QSettings{};
  }
}

QString Io::GetFile(DataFile file) {
  auto dir = QString{"://data/"};
  switch (file) {
    case DataFile::kSettings:
      return dir + "settings.ini";
    case DataFile::kWebSearches:
      return dir + "web-searches.json";
    default:
      return QString{};
  }
}

template <typename T>
void Io::ParseJsonToBaseResults(const QString& path) {
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
    autocompleter_.Insert(cmd);
    base_results_map_[cmd].push_back(result);
  }
}

void Io::UpdateDefaultBaseResults() {
  default_base_results_.clear();
  auto user_settings = GetFile(ConfigFile::kUser);

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
