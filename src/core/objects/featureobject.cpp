#include "featureobject.h"

#include <QDateTime>
#include <Qt>
#include <QtGlobal>
#include <limits>

#include "../models/featuremodel.h"
#include "../settings.h"

FeatureObject::FeatureObject(FeatureModel* model)
    : description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()},
      model_{model},
      title_{model->GetTitle()} {}

FeatureObject::FeatureObject(FeatureModel* model, const QString& input)
    : description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()},
      input_{input},
      model_{model},
      title_{model->GetTitle()} {}

FeatureObject::FeatureObject(FeatureModel* model, const QString& input,
                             const QString& argument)
    : description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()},
      input_{input},
      model_{model} {
  auto is_argument_empty = argument.isEmpty();

  if (auto title = model->GetTitle(); title.contains("%1")) {
    title_ =
      title.arg(is_argument_empty ? model->GetTitlePlaceholder() : argument);
  } else {
    title_ = title;
  }
}

QString FeatureObject::GetDescription() const { return description_; }

QPixmap FeatureObject::GetIcon() const { return icon_; }

uint64_t FeatureObject::GetId() const { return id_; }

QString FeatureObject::GetTitle() const { return title_; }

bool FeatureObject::operator<(const FeatureObject& rhs) const {
  // First, compare by score in descending order.
  if (auto ls = model_->CalculateScore(), rs = rhs.model_->CalculateScore();
      !qFuzzyCompare(ls, rs)) {
    // This is the proper way to compare if one double is greater than the
    // other.
    return ls - rs > std::numeric_limits<double>::epsilon();
  }

  // If scores are equal, compare by title in ascending order.
  return title_ < rhs.title_;
}

void FeatureObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      if (auto formatted_command = model_->FormatCommand();
          !input_.startsWith(formatted_command)) {
        emit NewSearchBoxTextRequested(formatted_command);
      }

      break;
    case Qt::Key_Return: {
      auto current_timestamp = QDateTime::currentSecsSinceEpoch();
      auto& settings = Settings::GetInstance();
      settings.AddUsageTime(id_, current_timestamp);
      settings.Save();

      // We set the usage timestamps on the model after saving and adding to the
      // settings file because the settings file will handle the maximum number
      // of timestamps being tracked at any given time.
      model_->SetTimestamps(settings.GetUsageTimes()[id_]);
      break;
    }
    default:
      break;
  }
}

QString FeatureObject::GetAltDescription() const {
  return model_->GetAltDescription();
}

QString FeatureObject::GetAltTitle() const { return model_->GetAltTitle(); }

FeatureModel* FeatureObject::GetModel() const { return model_; }

void FeatureObject::SetDescription(const QString& value) {
  description_ = value;
}

void FeatureObject::SetIcon(const QIcon& value) {
  icon_ = value.pixmap(Settings::GetInstance().GetSearchResultIconSize());
}

void FeatureObject::SetId(uint64_t value) { id_ = value; }

void FeatureObject::SetTitle(const QString& value) { title_ = value; }
