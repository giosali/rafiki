#include "featureobject.h"

#include "../models/featuremodel.h"

FeatureObject::FeatureObject(const FeatureModel* model)
    : alt_description_{model->GetAltDescription()},
      alt_title_{model->GetAltTitle()},
      description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()},
      title_{model->GetTitle()} {}

FeatureObject::FeatureObject(const FeatureModel* model, const QString& input)
    : alt_description_{model->GetAltDescription()},
      description_{model->GetDescription()},
      icon_{model->GetIcon()},
      id_{model->GetId()} {
  auto title = model->GetTitle();
  title_ = title.contains("%1") ? title.arg(input) : title;

  auto alt_title = model->GetAltTitle();
  alt_title_ = alt_title.contains("%1") ? alt_title.arg(input) : alt_title;
}

QString FeatureObject::GetDescription() const { return description_; }

QPixmap FeatureObject::GetIcon() const { return icon_; }

uint64_t FeatureObject::GetId() const { return id_; }

QString FeatureObject::GetTitle() const { return title_; }

void FeatureObject::SetDescription(const QString& value) {
  description_ = value;
}

void FeatureObject::SetIcon(const QPixmap& value) { icon_ = value; }

void FeatureObject::SetId(uint64_t value) { id_ = value; }

void FeatureObject::SetTitle(const QString& value) { title_ = value; }

QString FeatureObject::GetAltDescription() const { return alt_description_; }

QString FeatureObject::GetAltTitle() const { return alt_title_; }
