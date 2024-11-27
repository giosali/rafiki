#include "file.h"

#include <QDir>
#include <QFileInfo>
#include <QIODeviceBase>

void File::Copy(const QString& path, const QString& new_path) {
  auto new_file = QFile{new_path};
  MakeParents(new_file);

  QFile::copy(path, new_path);
}

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

void File::MakeParents(const QFile& file) {
  auto info = QFileInfo{file};
  auto dir = info.dir();
  dir.mkpath(dir.path());
}

void File::Write(const QString& path, const QJsonDocument& document) {
  auto file = QFile{path};
  MakeParents(file);

  file.open(QFile::WriteOnly);
  file.write(document.toJson(QJsonDocument::Compact));
}
