#include "paths.h"

#include <QStandardPaths>
#include <QtSystemDetection>

#include "../core/theme.h"
#include "utilities.h"

QString Paths::FormatIconPath(const QString& filename) {
  auto is_light_mode = Theme::GetInstance().GetIsLightMode();
  auto prefix = QString{is_light_mode ? ":/icons/light/" : ":/icons/dark/"};
  return prefix + filename;
}

QString Paths::GetPath(Directory d) {
  auto path = QString{};
  switch (d) {
    case Directory::kAppConfig:
      path =
        QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
      break;
    case Directory::kDataResources:
      path = "://data";
      break;
    case Directory::kUserIcons:
      path = Utilities::Combine(GetPath(Directory::kAppConfig), "icons");
      break;
  }

  return path;
}

QString Paths::GetPath(Json f) {
  auto directory = QString{};
  auto filename = QString{};
  switch (f) {
    case Json::kUserSettings:
      directory = GetPath(Directory::kAppConfig);
      filename = "settings.json";
      break;
    case Json::kUserWebSearches:
      directory = GetPath(Directory::kAppConfig);
      filename = "your-web-searches.json";
      break;
    case Json::kWebSearches:
      directory = GetPath(Directory::kDataResources);
      filename = "web-searches.json";
      break;
  }

  return Utilities::Combine(directory, filename);
}
