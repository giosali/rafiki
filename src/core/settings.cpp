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

void Settings::AddUseCount(uint64_t id, uint64_t count) {
  use_counts_[id] = count;
}

void Settings::ClearIgnoredDirectoryNames() {
  ignored_directory_names_.clear();
}

uint64_t Settings::GetAvailableId() const { return available_id_; }

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

std::unordered_map<uint64_t, uint64_t> Settings::GetUseCounts() const {
  return use_counts_;
}

void Settings::RemoveDisabledFeatureModelId(uint64_t id) {
  disabled_feature_model_ids_.erase(id);
}

void Settings::RemoveUseCount(uint64_t id) { use_counts_.erase(id); }

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

  auto use_counts = QJsonObject{};
  for (const auto& pair : use_counts_) {
    auto id = QString::number(pair.first);
    auto count = QString::number(pair.second);
    use_counts.insert(id, count);
  }

  auto object = QJsonObject{
    {"availableId", QString::number(available_id_)},
    {"defaultModels", default_models},
    {"disabledModels", disabled_models},
    {"ignoredDirectoryNames", ignored_directory_names},
    {"language", QString::number(language_)},
    {"territory", QString::number(territory_)},
    {"themeFilename", theme_filename_},
    {"useCounts", use_counts},
  };
  File::Write(Paths::GetPath(Paths::Json::kUserSettings), object);
}

void Settings::SetAvailableId(uint64_t value) { available_id_ = value; }

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

  if (auto key = "availableId"; object.contains(key)) {
    available_id_ = object[key].toString().toULongLong();
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

  if (auto key = "useCounts"; object.contains(key)) {
    auto use_counts = object[key].toObject();
    for (const auto& key : use_counts.keys()) {
      auto value = use_counts.value(key);
      auto id = key.toULongLong();
      auto count = value.toString().toULongLong();
      use_counts_.insert({id, count});
    }
  }
}
