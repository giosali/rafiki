#include "websearch.h"

#include <QDesktopServices>
#include <QJsonValue>
#include <QUrl>
#include <Qt>

#include "searchresultlist.h"
#include "utils.h"

WebSearch::WebSearch(const QJsonObject& object)
    : DataModel{object["id"].toString(),
                object["icon"].toString(),
                object["title"].toString(),
                object["alt"].toObject()["title"].toString(),
                object["description"].toString(),
                object["command"].toString(),
                object["placeholder"].toString()} {
  url_ = object["url"].toString();

  auto alt = object["alt"].toObject();
  alt_url_ = alt["url"].toString();
}

QString WebSearch::DragAndDrop() { return QString{}; }

void WebSearch::ProcessKeyPress(const QKeyCombination& combination,
                                QWidget* parent) {
  auto search_result_list = dynamic_cast<SearchResultList*>(parent);
  if (search_result_list == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Tab: {
      auto command = GetCommand(true);
      if (search_result_list->GetCmd() != command) {
        emit search_result_list->TextReceived(command);
      }

      break;
    }
    case Qt::Key_Return:
      if (combination.keyboardModifiers() & Qt::AltModifier) {
        ProcessUrl(alt_url_, search_result_list);
        break;
      }

      ProcessUrl(url_, search_result_list);
      break;
    case Qt::Key_Alt:
      search_result_list->CurrentSearchResult()->SetTitle(alt_title_);
      break;
  }
}

void WebSearch::ProcessKeyRelease(const QKeyCombination& combination,
                                  QWidget* parent) {
  auto search_result_list = dynamic_cast<SearchResultList*>(parent);
  if (search_result_list == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Alt:
      auto arg = search_result_list->GetArg();
      search_result_list->CurrentSearchResult()->SetTitle(GetTitle(arg));
      break;
  }
}

void WebSearch::ProcessUrl(const QString& url,
                           SearchResultList* search_result_list) const {
  if (!url.contains("{}")) {
    search_result_list->HideParent();
    QDesktopServices::openUrl(QUrl(url));
    return;
  }

  auto arg = search_result_list->GetArg();

  // Means arg is equal to: QString().
  if (arg.isNull()) {
    emit search_result_list->TextReceived(GetCommand(true));
    return;
  }

  // Means arg is equal to: QString("").
  if (arg.isEmpty()) {
    return;
  }

  search_result_list->HideParent();
  QDesktopServices::openUrl(QUrl(utils::Format(url_, arg)));
}
