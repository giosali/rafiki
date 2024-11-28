#pragma once

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "paths.h"

class File {
 public:
  File() = delete;

  static void Copy(const QString& path, const QString& new_path);
  static QJsonDocument Read(Paths::Json filename);
  static void Write(const QString& path, const QJsonArray& array);
  static void Write(const QString& path, const QJsonObject& object);

 private:
  static void MakeParents(const QFile& file);
  static void Write(const QString& path, const QJsonDocument& document);
};
