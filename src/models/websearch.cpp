#include "websearch.h"

#include <QDesktopServices>
#include <QJsonValue>
#include <QUrl>
#include <Qt>

#include "../core/config.h"
#include "../core/utils.h"

WebSearch::WebSearch(const QJsonObject& object)
    : is_custom_{false}, url_{object["url"].toString()} {
  auto alt = object["alt"].toObject();
  auto title = object["title"].toString();

  SetAltTitle(alt["title"].toString());
  SetAppendSpaceToCommand(ShouldAppendSpaceToCommand(title));
  SetCommand(object["command"].toString());
  SetIcon(object["icon"].toString());
  SetId(object["id"].toString());
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
                     const QString& alt_title,
                     const QString& alt_title_placeholder)
    : alt_url_{alt_url}, is_custom_{true}, url_{url} {
  SetAltTitle(alt_title);
  SetAltTitlePlaceholder(alt_title_placeholder);
  SetAppendSpaceToCommand(ShouldAppendSpaceToCommand(title));
  SetCommand(command);
  SetIcon(icon);
  SetTitle(title);
  SetTitlePlaceholder(title_placeholder);
}

bool WebSearch::IsCustom() const { return is_custom_; }

QJsonObject WebSearch::ToJsonObject() const {
  auto object = QJsonObject{};
  object.insert("id", GetId().ToString());
  object.insert("command", GetCommand());
  object.insert("icon", GetIcon());
  object.insert("url", url_);
  object.insert("title", GetTitle());
  object.insert("placeholder", GetTitlePlaceholder());
  object.insert("isCustom", is_custom_);

  auto alt_object = QJsonObject{};
  alt_object.insert("url", alt_url_);
  alt_object.insert("title", GetAltTitle());
  alt_object.insert("placeholder", GetAltTitlePlaceholder());
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
      if (!url.contains("{}")) {
        emit Hidden();
        QDesktopServices::openUrl(QUrl(url));
        break;
      }

      auto argument = input.Argument();

      // Means arg is equal to: QString().
      if (argument.isNull()) {
        emit NewSearchBoxTextRequested(FormatCommand());
        break;
      }

      // Means arg is equal to: QString("").
      if (argument.isEmpty()) {
        break;
      }

      emit Hidden();
      QDesktopServices::openUrl(QUrl(utils::Format(url, argument)));
      break;
    }
    case Qt::Key_Alt:
      emit NewTitleRequested(GetTitle());
      break;
  }
}

void WebSearch::ProcessKeyRelease(const QKeyCombination& combination,
                                  const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewTitleRequested(FormatTitle(input.Argument()));
      break;
  }
}

bool WebSearch::ShouldAppendSpaceToCommand(const QString& title) const {
  return title.contains("{}");
}
