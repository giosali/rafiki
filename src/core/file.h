#ifndef FILE_H
#define FILE_H

#include <QJsonDocument>
#include <filesystem>

#include "paths.h"

class File {
 public:
  File() = delete;

  static QJsonDocument Read(Paths::Json filename);
  static void Write(const std::filesystem::path& path,
                    const std::string& buffer);
};

#endif  // FILE_H
