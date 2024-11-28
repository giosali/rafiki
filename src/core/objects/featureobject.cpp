#include "featureobject.h"

#include <Qt>

#include "../models/featuremodel.h"

FeatureObject::FeatureObject(const FeatureModel* model)
    : description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()},
      model_{model},
      title_{model->GetTitle()} {}

FeatureObject::FeatureObject(const FeatureModel* model, const QString& input)
    : description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()},
      input_{input},
      model_{model},
      title_{model->GetTitle()} {}

FeatureObject::FeatureObject(const FeatureModel* model, const QString& input,
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
  // First, compare by count in descending order.
  if (auto left_count = model_->GetClickCount(),
      right_count = rhs.model_->GetClickCount();
      left_count != right_count) {
    return left_count > right_count;
  }

  // If counts are equal, compare by name in ascending order.
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
    default:
      break;
  }
}

QString FeatureObject::GetAltDescription() const {
  return model_->GetAltDescription();
}

QString FeatureObject::GetAltTitle() const { return model_->GetAltTitle(); }

void FeatureObject::SetDescription(const QString& value) {
  description_ = value;
}

void FeatureObject::SetIcon(const QPixmap& value) { icon_ = value; }

void FeatureObject::SetId(uint64_t value) { id_ = value; }

void FeatureObject::SetTitle(const QString& value) { title_ = value; }
