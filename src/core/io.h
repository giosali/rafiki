#ifndef PROJECT_H
#define PROJECT_H

#include <QJsonDocument>
#include <QSettings>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "../models/result.h"
#include "paths.h"

class Io {
 public:
  Io() = delete;

  static QSettings GetFile(Paths::Ini f);
  static QJsonDocument GetFile(Paths::Json f);
};

#endif  // PROJECT_H
