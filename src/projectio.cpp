#include "projectio.h"

#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <iterator>
#include <utility>

#include "websearch.h"

ProjectIO::ProjectIO() {
  ParseDataFile<WebSearch>(GetDataFilePath(DataFile::kWebSearches));
}

QString ProjectIO::GetDataFilePath(DataFile file) {
  auto filename = QString();
  switch (file) {
    case DataFile::kWebSearches:
      filename = "web-searches.json";
      break;
  }

  auto dir = QString("://data/");
  return dir + filename;
}

QString ProjectIO::GetImageFilePath(ImageFile file) {
  auto filename = QString();
  switch (file) {
    case ImageFile::kQuestionMark:
      filename = "question-mark.png";
      break;
  }

  auto dir = QString("://images/");
  return dir + filename;
}

std::vector<std::unique_ptr<DataModel>> ProjectIO::FindDataModels(
    const QString& input) {
  auto suggestions = autocomplete_map_.Find(input);
  auto data_models = std::vector<std::unique_ptr<DataModel>>();
  for (const auto& suggestion : suggestions) {
    auto data_model_it = data_model_map_.find(suggestion);
    if (data_model_it == data_model_map_.end()) {
      continue;
    }

    auto dms = std::move(data_model_it->second);
    data_models.insert(data_models.end(), std::make_move_iterator(dms.begin()),
                       std::make_move_iterator(dms.end()));
  }

  return data_models;
}

template <typename T>
void ProjectIO::ParseDataFile(const QString& path) {
  auto file = QFile(path);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  auto doc = QJsonDocument::fromJson(file.readAll());

  auto arr = doc.array();
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    auto data_model = std::make_unique<T>();
    data_model->Populate(it->toObject());

    auto key = data_model->GetKey();
    autocomplete_map_.Insert(key);
    data_model_map_[key].push_back(std::move(data_model));
  }
}
