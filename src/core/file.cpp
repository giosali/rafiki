#include "file.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODeviceBase>

QJsonDocument File::Read(Paths::Json filename) {
  auto file = QFile{Paths::GetPath(filename)};
  if (!file.exists()) {
    return {};
  }

  file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text);
  auto document = QJsonDocument::fromJson(file.readAll());
  return document;
}

void File::Write(const QString& path, const QJsonArray& array) {
  auto document = QJsonDocument{};
  document.setArray(array);
  Write(path, document);
}

void File::Write(const QString& path, const QJsonObject& object) {
  auto document = QJsonDocument{};
  document.setObject(object);
  Write(path, document);
}

void File::Write(const QString& path, const QJsonDocument& document) {
  // Creates all necessary parent directories.
  QDir{}.mkpath(QFileInfo{path}.path());

  auto file = QFile{path};
  file.open(QFile::WriteOnly);
  file.write(document.toJson(QJsonDocument::Compact));
}
