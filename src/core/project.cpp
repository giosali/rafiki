#include "project.h"

#include <QCoreApplication>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUuid>
#include <iterator>
#include <set>

#include "../models/calculator.h"
#include "../models/trash.h"
#include "../models/websearch.h"

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
    case defs::ImageFile::kQuestionMark:
      return dir + "question-mark.png";
    case defs::ImageFile::kTrash:
      return dir + "trash.svg";
    default:
      return QString{};
  }
}

void Project::Initialize() {
  // Sets up default settings.
  auto default_settings = GetUserSettings();
  auto user_settings = GetDefaultSettings();

  for (const auto& group : default_settings.childGroups()) {
    SetMissingSettings(default_settings, user_settings, group);
  }

  // Sets up search results based on data files.
  ParseJsonToBaseResults<WebSearch>(
    GetDataFilePath(defs::DataFile::kWebSearches));

  // Sets up built-in search results not based on data files.
  AddBaseResult(std::make_shared<Trash>());
  AddProcessedBaseResult(std::make_shared<Calculator>());

  // Sets up default search results.
  UpdateDefaultBaseResults();
}

void Project::AddBaseResult(const std::shared_ptr<BaseResult>& base_result) {
  if (!base_result->HasCommand()) {
    return;
  }

  auto cmd = base_result->FormatCommand();
  autocomplete_.Insert(cmd);
  base_results_map_[cmd].push_back(base_result);
}

void Project::AddProcessedBaseResult(
  const std::shared_ptr<BaseResult>& base_result) {
  processed_base_results_.push_back(base_result);
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
