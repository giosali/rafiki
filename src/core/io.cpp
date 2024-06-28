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
#include <cstdint>
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
#include "utils.h"

std::vector<std::shared_ptr<Result>> Io::FindResults(const Input& input) {
  auto all_results = std::vector<std::shared_ptr<Result>>{};

  // Regular commands.
  auto suggestions = autocompleter_.Find(input);
  for (const auto& suggestion : suggestions) {
    auto results_it = results_map_.find(suggestion);
    if (results_it == results_map_.end()) {
      continue;
    }

    auto results = results_it->second;
    all_results.insert(all_results.end(), results.begin(), results.end());
  }

  // Input-dependent results.
  for (const auto& pbr : processed_results_) {
    if (!pbr->ProcessInput(input)) {
      continue;
    }

    all_results.push_back(pbr);
  }

  // Input-dependent results that also produce results.
  for (const auto& prb : processed_result_builders_) {
    if (!prb->ProcessInput(input)) {
      continue;
    }

    auto results = prb->GetResults();
    if (results.empty()) {
      all_results.push_back(prb);
    } else {
      all_results.insert(all_results.end(), results.begin(), results.end());
    }
  }

  return all_results;
}

std::vector<std::shared_ptr<Result>> Io::GetDefaultResults() {
  return default_results_;
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
      desktop_entry.GetPath(), desktop_entry.GetName(), desktop_entry.GetIcon(),
      desktop_entry.GetIconSize(), desktop_entry.GetExec());
    auto cmd = application->FormatCommand();
    autocompleter_.Insert(cmd);
    results_map_[cmd].push_back(application);
  }

  // Fetches icon theme on Linux (for GNOME) and adds MIME types.
  // For more info:
  // https://stackoverflow.com/a/44629154
  // https://github.com/cerebroapp/cerebro/issues/17
  mimetype_icons_ = gnulinux::Io::GetMimeTypeIcons();
#endif

  // Sets up search results based on data files.
  ParseJson<WebSearch>(GetFile(DataFile::kWebSearches));

  // Sets up built-in search results not based on data files.
  AddResult(std::make_shared<Trash>());
  AddProcessedResult(std::make_shared<Calculator>());
  AddProcessedResult(std::make_shared<Url>());
  AddProcessedResultBuilder(std::make_shared<FileSystemEntry>());

  // Sets up default search results.
  UpdateDefaultResults();
}

void Io::ToggleResult(uint64_t id, bool enable) {
  auto it = results_.find(id);
  if (it == results_.end()) {
    return;
  }

  auto result = it->second;
  result->SetIsEnabled(enable);

  auto user_settings = GetFile(ConfigFile::kUser);
  user_settings.beginGroup("Results");
  auto id_string = QString::number(id);
  auto disabled_ids = user_settings.value("DisabledIDs", "")
                        .toString()
                        .split(',', Qt::SkipEmptyParts);

  // If we're enabling the result associated with the given ID, we need to
  // remove it from the list of currently disabled IDs.
  if (enable) {
    if (auto i = disabled_ids.indexOf(id_string); i != -1) {
      disabled_ids.removeAt(i);
    }
  } else {
    // Otherwise, since we're disabling the result, we need to add it to the
    // list of currently disabled IDs.
    disabled_ids.push_back(id_string);
  }

  // Queues saving disabled IDs to file.
  user_settings.setValue("DisabledIDs", disabled_ids.join(','));
}

void Io::AddResult(const std::shared_ptr<Result>& result) {
  results_.insert({result->GetId(), result});

  if (!result->HasCommand()) {
    return;
  }

  auto cmd = result->FormatCommand();
  autocompleter_.Insert(cmd);
  results_map_[cmd].push_back(result);
}

void Io::AddProcessedResult(const std::shared_ptr<ProcessedResult>& result) {
  results_.insert({result->GetId(), result});

  if (result->HasCommand()) {
    auto cmd = result->FormatCommand();
    autocompleter_.Insert(cmd);
  }
}

void Io::AddProcessedResultBuilder(
  const std::shared_ptr<ProcessedResultBuilder>& builder) {
  results_.insert({builder->GetId(), builder});
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
void Io::ParseJson(const QString& path) {
  auto file = QFile{path};
  if (!file.exists()) {
    return;
  }

  file.open(QIODevice::ReadOnly | QIODevice::Text);
  auto document = QJsonDocument::fromJson(file.readAll());
  if (document.isObject()) {
    AddResult(std::make_shared<T>(document.object()));
  } else if (document.isArray()) {
    for (const auto& i : document.array()) {
      AddResult(std::make_shared<T>(i.toObject()));
    }
  }
}

void Io::UpdateDefaultResults() {
  default_results_.clear();
  auto user_settings = GetFile(ConfigFile::kUser);

  // Takes the IDs of the default search results and stores them in a set.
  user_settings.beginGroup("DefaultSearchResults");

  auto ids = std::set<uint64_t>();
  for (const auto& key : user_settings.allKeys()) {
    ids.insert(user_settings.value(key).toULongLong());
  }

  user_settings.endGroup();

  for (const auto& [_, results] : results_map_) {
    for (const auto& result : results) {
      // TODO: update syntax in C++20.
      if (ids.find(result->GetId()) == ids.end()) {
        continue;
      }

      default_results_.push_back(result);
    }
  }
}

Autocompleter Io::autocompleter_{};

std::vector<std::shared_ptr<Result>> Io::default_results_{};

std::unordered_map<std::string, QString> Io::mimetype_icons_{};

std::vector<std::shared_ptr<ProcessedResultBuilder>>
  Io::processed_result_builders_{};

std::vector<std::shared_ptr<ProcessedResult>> Io::processed_results_{};

std::unordered_map<uint64_t, std::shared_ptr<Result>> Io::results_{};

std::unordered_map<QString, std::vector<std::shared_ptr<Result>>>
  Io::results_map_{};
