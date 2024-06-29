#include "websearch.h"

#include <QDesktopServices>
#include <QJsonValue>
#include <QUrl>
#include <Qt>

#include "../core/utils.h"

WebSearch::WebSearch(const QJsonObject& object)
    : Result{object["id"].toString().toULongLong(),
             object["icon"].toString(),
             object["title"].toString(),
             object["placeholder"].toString(),
             object["alt"].toObject()["title"].toString(),
             object["description"].toString(),
             object["command"].toString(),
             object["title"].toString().contains("{}")},
      is_custom_{false} {
  url_ = object["url"].toString();

  auto alt = object["alt"].toObject();
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
      emit NewTitleRequested(alt_title_);
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
