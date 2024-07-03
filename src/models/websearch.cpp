#include "websearch.h"

#include <QDesktopServices>
#include <QJsonValue>
#include <QUrl>
#include <Qt>

#include "../core/utils.h"

WebSearch::WebSearch(const QJsonObject& object)
    : is_custom_{false}, url_{object["url"].toString()} {
  auto alt = object["alt"].toObject();
  auto title = object["title"].toString();

  SetAltTitle(alt["title"].toString());
  SetAppendSpaceToCommand(title.contains("{}"));
  SetCommand(object["command"].toString());
  SetIcon(object["icon"].toString());
  SetId(object["id"].toString().toULongLong());
  SetTitle(title);
  SetTitlePlaceholder(object["placeholder"].toString());

  alt_url_ = alt["url"].toString();

  if (auto is_custom_value = object["isCustom"];
      !is_custom_value.isUndefined()) {
    is_custom_ = is_custom_value.toBool();
  }
}

bool WebSearch::IsCustom() const { return is_custom_; }

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
      emit NewTitleRequested(Title());
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
