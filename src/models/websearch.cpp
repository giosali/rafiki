#include "websearch.h"

#include <QDesktopServices>
#include <QJsonValue>
#include <QUrl>
#include <Qt>

WebSearch::WebSearch(const QJsonObject& object)
    : is_custom_{false}, url_{object["url"].toString()} {
  auto alt = object["alt"].toObject();
  auto title = object["title"].toString();

  SetAltTitle(alt["title"].toString());
  SetAppendSpaceToCommand(ShouldAppendSpaceToCommand(title));
  SetCommand(object["command"].toString());
  SetId(object["id"].toString().toULongLong());
  SetPixmap(object["icon"].toString());
  SetTitle(title);
  SetTitlePlaceholder(object["placeholder"].toString());

  alt_url_ = alt["url"].toString();

  if (auto v = object["isCustom"]; !v.isUndefined()) {
    is_custom_ = v.toBool();
  }
}

WebSearch::WebSearch(const QString& url, const QString& title,
                     const QString& title_placeholder, const QString& command,
                     const QString& icon, const QString& alt_url,
                     const QString& alt_title)
    : alt_url_{alt_url}, is_custom_{true}, url_{url} {
  SetAltTitle(alt_title);
  SetAppendSpaceToCommand(ShouldAppendSpaceToCommand(title));
  SetCommand(command);
  SetPixmap(icon);
  SetTitle(title);
  SetTitlePlaceholder(title_placeholder);
}

QString WebSearch::GetAltUrl() const { return alt_url_; }

QString WebSearch::GetUrl() const { return url_; }

bool WebSearch::IsCustom() const { return is_custom_; }

void WebSearch::SetAltUrl(const QString& value) { alt_url_ = value; }

void WebSearch::SetUrl(const QString& value) { url_ = value; }

QJsonObject WebSearch::ToJsonObject() const {
  auto object = QJsonObject{};
  object.insert("id", QString::number(GetId()));
  object.insert("command", GetCommand());
  object.insert("icon", GetPixmapPath());
  object.insert("url", url_);
  object.insert("title", GetTitle());
  object.insert("placeholder", GetTitlePlaceholder());
  object.insert("isCustom", is_custom_);

  auto alt_object = QJsonObject{};
  alt_object.insert("url", alt_url_);
  alt_object.insert("title", GetAltTitle());
  object.insert("alt", alt_object);

  return object;
}

void WebSearch::Drag() {}

void WebSearch::ProcessKeyPress(const QKeyCombination& combination,
                                const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      if (auto command = FormatCommand(); input.Command() != command) {
        emit NewSearchBoxTextRequested(command);
      }

      break;
    case Qt::Key_Return: {
      auto url =
        combination.keyboardModifiers() & Qt::AltModifier ? alt_url_ : url_;
      if (!url.contains("%1")) {
        emit Hidden();
        QDesktopServices::openUrl(QUrl{url});
        break;
      }

      // Means arg is equal to: QString().
      auto argument = input.Argument();
      if (argument.isNull()) {
        emit NewSearchBoxTextRequested(FormatCommand());
        break;
      }

      // Means arg is equal to: QString("").
      if (argument.isEmpty()) {
        break;
      }

      emit Hidden();
      QDesktopServices::openUrl(QUrl{url.arg(argument)});
      break;
    }
    case Qt::Key_Alt:
      emit NewTitleRequested(GetAltTitle());
      break;
    default:
      break;
  }
}

void WebSearch::ProcessKeyRelease(const QKeyCombination& combination,
                                  const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewTitleRequested(FormatTitle(input.Argument()));
      break;
    default:
      break;
  }
}

bool WebSearch::ShouldAppendSpaceToCommand(const QString& title) const {
  return title.contains("{}");
}
