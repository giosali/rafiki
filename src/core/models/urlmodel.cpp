#include "urlmodel.h"

#include <memory>

#include "../bridges/urlbridge.h"
#include "../visitors/featurevisitor.h"

UrlModel::UrlModel() : FeatureModel{std::make_unique<UrlBridge>()} {
  SetDescription(tr("Open URL"));
  SetIcon(":/icons/url.svg");
  SetId(19);
}

void UrlModel::Accept(FeatureVisitor& v) { v.Visit(this); }
