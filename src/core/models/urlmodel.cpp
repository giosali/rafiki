#include "urlmodel.h"

#include <memory>

#include "../bridges/urlbridge.h"
#include "../paths.h"
#include "../visitors/featurevisitor.h"

UrlModel::UrlModel() : FeatureModel{std::make_unique<UrlBridge>()} {
  SetDescription("Open URL");
  SetIcon(Paths::GetPath(Paths::Image::kUrl));
  SetId(19);
}

void UrlModel::Accept(FeatureVisitor& v) { v.Visit(this); }
