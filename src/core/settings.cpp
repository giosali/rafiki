#include "settings.h"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>

#include "file.h"
#include "paths.h"

Settings& Settings::GetInstance() {
  // Thread-safe magic static.
  static auto instance = Settings{};
  return instance;
}

void Settings::AddDisabledFeatureModelId(uint64_t id) {
  disabled_feature_model_ids_.insert(id);
}

void Settings::AddIgnoredDirectoryName(const std::string& name) {
  ignored_directory_names_.insert(name);
}

void Settings::AddUsageTime(uint64_t id, uint64_t timestamp) {
  auto& timestamps = usage_times_[id];
  timestamps.push_back(timestamp);
  if (timestamps.size() > 30) {
    // Removes the first element from the vector.
    timestamps.erase(timestamps.begin());
  }
}

void Settings::ClearIgnoredDirectoryNames() {
  ignored_directory_names_.clear();
}

std::vector<uint64_t> Settings::GetDefaultFeatureModelIds() const {
  return default_feature_model_ids_;
}

std::unordered_set<uint64_t> Settings::GetDisabledFeatureModelids() const {
  return disabled_feature_model_ids_;
}

std::unordered_set<std::string> Settings::GetIgnoredDirectoryNames() const {
  return ignored_directory_names_;
}

QLocale::Language Settings::GetLanguage() const { return language_; }

int Settings::GetSearchResultIconSize() const {
  return search_result_icon_size_;
}

int Settings::GetSearchResultListMaxCount() const {
  return search_result_list_max_count_;
}

int Settings::GetSearchResultTitleMaxHeight() const {
  return search_result_title_max_height_;
}

QLocale::Territory Settings::GetTerritory() const { return territory_; }

QString Settings::GetThemeFilename() const { return theme_filename_; }

std::unordered_map<uint64_t, std::vector<uint64_t>> Settings::GetUsageTimes()
  const {
  return usage_times_;
}

void Settings::Initialize() { Update(File::Read(Paths::Json::kUserSettings)); }

void Settings::RemoveDisabledFeatureModelId(uint64_t id) {
  disabled_feature_model_ids_.erase(id);
}

void Settings::RemoveUsageTimes(uint64_t id) { usage_times_.erase(id); }

void Settings::Save() const {
  auto default_models = QJsonArray{};
  for (auto id : default_feature_model_ids_) {
    default_models.append(QString::number(id));
  }

  auto disabled_models = QJsonArray{};
  for (auto id : disabled_feature_model_ids_) {
    disabled_models.append(QString::number(id));
  }

  auto ignored_directory_names = QJsonArray{};
  for (const auto& name : ignored_directory_names_) {
    ignored_directory_names.append(QString::fromStdString(name));
  }

  auto usage_times = QJsonObject{};
  for (const auto& pair : usage_times_) {
    auto id = QString::number(pair.first);
    auto timestamps = QJsonArray{};
    for (auto time : pair.second) {
      timestamps.append(QString::number(time));
    }

    usage_times.insert(id, timestamps);
  }

  auto object = QJsonObject{
    {"defaultModels", default_models},
    {"disabledModels", disabled_models},
    {"ignoredDirectoryNames", ignored_directory_names},
    {"language", QString::number(language_)},
    {"territory", QString::number(territory_)},
    {"themeFilename", theme_filename_},
    {"usageTimes", usage_times},
  };
  File::Write(Paths::GetPath(Paths::Json::kUserSettings), object);
}

void Settings::SetLanguage(QLocale::Language value) {
  language_ = value;
  emit LocaleChanged();
}

void Settings::SetTerritory(QLocale::Territory value) {
  territory_ = value;
  emit LocaleChanged();
}

void Settings::SetThemeFilename(const QString& value) {
  theme_filename_ = value;
}

void Settings::Update(const QJsonDocument& document) {
  auto object = document.object();
  if (object.isEmpty()) {
    return;
  }

  if (auto key = "defaultModels"; object.contains(key)) {
    default_feature_model_ids_.clear();
    for (const auto& value : object[key].toArray()) {
      default_feature_model_ids_.push_back(value.toString().toULongLong());
    }
  }

  if (auto key = "disabledModels"; object.contains(key)) {
    disabled_feature_model_ids_.clear();
    for (const auto& value : object[key].toArray()) {
      disabled_feature_model_ids_.insert(value.toString().toULongLong());
    }
  }

  if (auto key = "ignoredDirectoryNames"; object.contains(key)) {
    ClearIgnoredDirectoryNames();
    for (const auto& value : object[key].toArray()) {
      AddIgnoredDirectoryName(value.toString().toStdString());
    }
  }

  if (auto key = "language"; object.contains(key)) {
    language_ = static_cast<QLocale::Language>(object[key].toString().toInt());
  }

  if (auto key = "territory"; object.contains(key)) {
    territory_ =
      static_cast<QLocale::Territory>(object[key].toString().toInt());
  }

  if (auto key = "themeFilename"; object.contains(key)) {
    theme_filename_ = object[key].toString();
  }

  if (auto key = "usageTimes"; object.contains(key)) {
    auto usage_times = object[key].toObject();
    for (const auto& key : usage_times.keys()) {
      auto value = usage_times.value(key);
      auto id = key.toULongLong();

      auto timestamps = std::vector<uint64_t>{};
      for (const auto& time : value.toArray()) {
        timestamps.push_back(time.toString().toULongLong());
      }

      usage_times_.insert({id, timestamps});
    }
  }
}
