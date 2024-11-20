#include "websearchmodel.h"

#include <memory>

#include "../bridges/websearchbridge.h"
#include "../visitors/featurevisitor.h"

WebSearchModel::WebSearchModel(const QJsonObject& object)
    : FeatureModel{std::make_unique<WebSearchBridge>()},
      url_{object["url"].toString()} {
  auto alt = object["alt"].toObject();
  auto title = object["title"].toString();

  SetAltTitle(alt["title"].toString());
  SetCommand(object["command"].toString());
  SetIcon(object["icon"].toString());
  SetId(object["id"].toString().toULongLong());
  SetTitle(title);
  SetTitlePlaceholder(object["placeholder"].toString());

  alt_url_ = alt["url"].toString();

  if (auto v = object["isCustom"]; !v.isUndefined()) {
    is_custom_ = v.toBool();
  }
}

void WebSearchModel::Accept(FeatureVisitor& v) { v.Visit(this); }

bool WebSearchModel::ReceivesInput() const { return url_.contains("%1"); }

QString WebSearchModel::GetAltUrl() const { return alt_url_; }

QString WebSearchModel::GetUrl() const { return url_; }
