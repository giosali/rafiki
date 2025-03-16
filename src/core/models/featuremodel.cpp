#include "featuremodel.h"

#include <QDateTime>
#include <QFile>
#include <cmath>
#include <utility>

#include "../core/utilities.h"
#include "../settings.h"

QString FeatureModel::FormatCommand() const {
  return ReceivesInput() ? command_ + " " : command_;
}

FeatureModel::FeatureModel(std::unique_ptr<FeatureBridge> bridge)
    : feature_bridge_{std::move(bridge)} {}

bool FeatureModel::ReceivesInput() const { return title_.contains("%1"); }

std::unordered_set<std::string> FeatureModel::Tokenize() const {
  return std::unordered_set<std::string>{
    Utilities::RemoveAccents(FormatCommand()).toLower().toStdString()};
}

double FeatureModel::CalculateScore() const {
  auto score = double{};
  auto current_timestamp = QDateTime::currentSecsSinceEpoch();
  for (auto timestamp : timestamps_) {
    auto diff = current_timestamp - timestamp;
    auto ratio = static_cast<double>(diff) / kHalfLife;
    auto decay_factor = std::pow(kDecayBase, ratio);
    score += decay_factor;
  }

  return score;
}

QString FeatureModel::GetAltDescription() const { return alt_description_; }

QString FeatureModel::GetAltTitle() const { return alt_title_; }

QString FeatureModel::GetCommand() const { return command_; }

QString FeatureModel::GetDescription() const { return description_; }

FeatureBridge* FeatureModel::GetFeatureBridge() const {
  return feature_bridge_.get();
}

QPixmap FeatureModel::GetIcon() const { return icon_; }

uint64_t FeatureModel::GetId() const { return id_; }

bool FeatureModel::GetIsEnabled() const { return is_enabled_; }

std::vector<uint64_t> FeatureModel::GetTimestamps() const {
  return timestamps_;
}

QString FeatureModel::GetTitle() const { return title_; }

QString FeatureModel::GetTitlePlaceholder() const { return title_placeholder_; }

void FeatureModel::SetIsEnabled(bool value) { is_enabled_ = value; }

void FeatureModel::SetTimestamps(const std::vector<uint64_t>& value) {
  timestamps_ = value;
}

void FeatureModel::SetAltDescription(const QString& value) {
  alt_description_ = value;
}

void FeatureModel::SetAltTitle(const QString& value) { alt_title_ = value; }

void FeatureModel::SetCommand(const QString& value) { command_ = value; }

void FeatureModel::SetDescription(const QString& value) {
  description_ = value;
}

void FeatureModel::SetIcon(const QString& value) {
  auto icon = QFile::exists(value)
                ? QIcon{value}
                : QIcon::fromTheme(value, QIcon{":/icons/question-mark.png"});
  icon_ = icon.pixmap(Settings::GetInstance().GetSearchResultIconSize());
}

void FeatureModel::SetId(uint64_t value) { id_ = value; }

void FeatureModel::SetTitle(const QString& value) { title_ = value; }

void FeatureModel::SetTitlePlaceholder(const QString& value) {
  title_placeholder_ = value;
}
