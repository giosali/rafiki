#ifndef FILE_H
#define FILE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "paths.h"

class File {
 public:
  File() = delete;

  static QJsonDocument Read(Paths::Json filename);
  static void Write(const QString& path, const QJsonArray& array);
  static void Write(const QString& path, const QJsonObject& object);

 private:
  static void Write(const QString& path, const QJsonDocument& document);
};

#endif  // FILE_H
