#ifndef FILE_H
#define FILE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

#include "paths.h"

class File {
 public:
  File() = delete;

  static QJsonDocument Read(Paths::Json filename);
  static void Write(const QString& path, const QJsonArray& array);
};

#endif  // FILE_H
