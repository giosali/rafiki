#include "io.h"

#include <QFile>
#include <QFileInfo>
#include <QIODeviceBase>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

QSettings Io::GetFile(Paths::Ini f) {
  auto path = Paths::Path(f);
  return path.isEmpty() ? QSettings{} : QSettings{path, QSettings::IniFormat};
}

QJsonDocument Io::GetFile(Paths::Json f) {
  auto file = QFile{Paths::Path(f)};
  if (!file.exists()) {
    return QJsonDocument{};
  }

  file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text);
  auto document = QJsonDocument::fromJson(file.readAll());
  return document;
}
