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
  auto file = QFile{path};

  // Creates any missing parent directories.
  auto info = QFileInfo{file};
  auto dir = info.dir();
  dir.mkpath(dir.path());

  file.open(QFile::WriteOnly);
  file.write(document.toJson(QJsonDocument::Compact));
}
