#include "projectio.h"

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
#include <utility>

#include "websearch.h"

ProjectIO::ProjectIO() {
  auto internal_settings =
    QSettings{GetDataFilePath(DataFile::kSettings), QSettings::IniFormat};
  auto external_settings = QSettings{QSettings::IniFormat, QSettings::UserScope,
                                     QCoreApplication::organizationName(),
                                     QCoreApplication::applicationName()};

  for (const auto& group : internal_settings.childGroups()) {
    SetMissingSettings(internal_settings, external_settings, group);
  }

  ParseDataFile<WebSearch>(GetDataFilePath(DataFile::kWebSearches));
  PopulateDefaultBaseResults(external_settings);
}

QString ProjectIO::GetDataFilePath(DataFile file) {
  auto filename = QString{};
  switch (file) {
    case DataFile::kSettings:
      filename = "settings.ini";
      break;
    case DataFile::kWebSearches:
      filename = "web-searches.json";
      break;
  }

  auto dir = QString{"://data/"};
  return dir + filename;
}

QString ProjectIO::GetImageFilePath(ImageFile file) {
  auto filename = QString{};
  switch (file) {
    case ImageFile::kCalculator:
      filename = "calculator.png";
      break;
    case ImageFile::kQuestionMark:
      filename = "question-mark.png";
      break;
  }

  auto dir = QString{"://images/"};
  return dir + filename;
}

std::vector<std::shared_ptr<BaseResult>> ProjectIO::FindBaseResults(
  const QString& cmd) {
  auto data_models_concat = std::vector<std::shared_ptr<BaseResult>>{};
  auto suggestions = autocomplete_map_.Find(cmd);
  for (const auto& suggestion : suggestions) {
    auto data_models_it = data_models_map_.find(suggestion);
    if (data_models_it == data_models_map_.end()) {
      continue;
    }

    auto data_models = data_models_it->second;
    data_models_concat.insert(data_models_concat.end(), data_models.begin(),
                              data_models.end());
  }

  return data_models_concat;
}

std::vector<std::shared_ptr<BaseResult>> ProjectIO::GetDefaultBaseResults() {
  return default_data_models_;
}

template <typename T>
void ProjectIO::ParseDataFile(const QString& path) {
  auto file = QFile{path};
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  auto doc = QJsonDocument::fromJson(file.readAll());

  auto arr = doc.array();
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    auto data_model = std::make_unique<T>(it->toObject());
    auto cmd = data_model->GetCommand(true);
    autocomplete_map_.Insert(cmd);
    data_models_map_[cmd].push_back(std::move(data_model));
  }
}

void ProjectIO::PopulateDefaultBaseResults(QSettings& external_settings) {
  external_settings.beginGroup("DefaultSearchBaseResults");

  auto guid_set = std::set<QUuid>();
  for (const auto& key : external_settings.allKeys()) {
    guid_set.insert(QUuid::fromString(external_settings.value(key).toString()));
  }

  for (const auto& [_, value] : data_models_map_) {
    for (auto i : value) {
      if (guid_set.find(i->GetId()) == guid_set.end()) {
        continue;
      }

      default_data_models_.push_back(i);
    }
  }

  external_settings.endGroup();
}

void ProjectIO::SetMissingSettings(QSettings& internal_settings,
                                   QSettings& external_settings,
                                   const QString& group) const {
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
