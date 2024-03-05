#include "projectio.h"

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

std::vector<std::shared_ptr<DataModel>> ProjectIO::FindDataModels(
    const QString& cmd) {
  auto data_models_concat = std::vector<std::shared_ptr<DataModel>>();
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

template <typename T>
void ProjectIO::ParseDataFile(const QString& path) {
  auto file = QFile(path);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  auto doc = QJsonDocument::fromJson(file.readAll());

  auto arr = doc.array();
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    auto data_model = std::make_unique<T>();
    data_model->Populate(it->toObject());

    auto cmd = data_model->GetCommand(true);
    autocomplete_map_.Insert(cmd);
    data_models_map_[cmd].push_back(std::move(data_model));
  }
}
