#include "websearchmodel.h"

#include <memory>

#include "../bridges/websearchbridge.h"
#include "../core/paths.h"
#include "../visitors/featurevisitor.h"

WebSearchModel::WebSearchModel(const QJsonObject& object)
    : FeatureModel{std::make_unique<WebSearchBridge>()} {
  if (object.contains("isCustom") &&
      (is_custom_ = object["isCustom"].toBool())) {
    SetCommand(object["command"].toString());
    SetIcon(object["icon"].toString());
    SetId(object["id"].toString().toULongLong());
    SetTitle(object["title"].toString());
    SetTitlePlaceholder(object["placeholder"].toString());
    SetUrl(object["url"].toString());

    if (object.contains("alt")) {
      auto alt = object["alt"].toObject();
      SetAltTitle(alt["title"].toString());
      alt_url_ = alt["url"].toString();
    }
  } else {
    SetCommand(object["command"].toString());

    auto icon = object["icon"].toString();
    SetIcon(icon.startsWith(":/icons/") ? icon : Paths::FormatIconPath(icon));

    SetId(object["id"].toString().toULongLong());
    SetTitle(tr(object["title"].toString().toStdString().c_str()));
    SetTitlePlaceholder(object["placeholder"].toString());
    SetUrl(tr(object["url"].toString().toStdString().c_str()));

    if (object.contains("alt")) {
      auto alt = object["alt"].toObject();
      SetAltTitle(tr(alt["title"].toString().toStdString().c_str()));
      alt_url_ = tr(alt["url"].toString().toStdString().c_str());
    }
  }
}

void WebSearchModel::Accept(FeatureVisitor& v) { v.Visit(this); }

bool WebSearchModel::ReceivesInput() const { return url_.contains("%1"); }

QString WebSearchModel::GetAltUrl() const { return alt_url_; }

QString WebSearchModel::GetIconPath() const { return icon_path_; }

bool WebSearchModel::GetIsCustom() const { return is_custom_; }

QString WebSearchModel::GetUrl() const { return url_; }

void WebSearchModel::SetAltTitle(const QString& value) {
  FeatureModel::SetAltTitle(value);
}

void WebSearchModel::SetAltUrl(const QString& value) { alt_url_ = value; }

void WebSearchModel::SetCommand(const QString& value) {
  FeatureModel::SetCommand(value);
}

void WebSearchModel::SetIcon(const QString& value) {
  FeatureModel::SetIcon(value);

  icon_path_ = value;
}

void WebSearchModel::SetTitle(const QString& value) {
  FeatureModel::SetTitle(value);
}

void WebSearchModel::SetTitlePlaceholder(const QString& value) {
  FeatureModel::SetTitlePlaceholder(value);
}

void WebSearchModel::SetUrl(const QString& value) { url_ = value; }

QJsonObject WebSearchModel::ToJson() const {
  return QJsonObject{
    {"id", QString::number(GetId())},
    {"command", GetCommand()},
    {"icon", GetIconPath()},
    {"url", url_},
    {"title", GetTitle()},
    {"placeholder", GetTitlePlaceholder()},
    {"isCustom", is_custom_},
    {"alt", QJsonObject{{"url", alt_url_}, {"title", GetAltTitle()}}}};
}
