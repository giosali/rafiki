#include "featureobject.h"

#include <Qt>

#include "../models/featuremodel.h"

FeatureObject::FeatureObject(const FeatureModel* model)
    : alt_description_{model->GetAltDescription()},
      alt_title_{model->GetAltTitle()},
      description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()},
      model_{model},
      title_{model->GetTitle()} {}

FeatureObject::FeatureObject(const FeatureModel* model, const QString& input)
    : alt_description_{model->GetAltDescription()},
      alt_title_{model->GetAltTitle()},
      description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()},
      input_{input},
      model_{model},
      title_{model->GetTitle()} {}

FeatureObject::FeatureObject(const FeatureModel* model, const QString& input,
                             const QString& argument)
    : alt_description_{model->GetAltDescription()},
      alt_title_{model->GetAltTitle()},
      description_{model->GetDescription()},
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

QString FeatureObject::GetAltDescription() const { return alt_description_; }

QString FeatureObject::GetAltTitle() const { return alt_title_; }

void FeatureObject::SetDescription(const QString& value) {
  description_ = value;
}

void FeatureObject::SetIcon(const QPixmap& value) { icon_ = value; }

void FeatureObject::SetId(uint64_t value) { id_ = value; }

void FeatureObject::SetTitle(const QString& value) { title_ = value; }
