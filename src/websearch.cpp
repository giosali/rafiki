#include "websearch.h"

#include <QDesktopServices>
#include <QJsonValue>
#include <QUrl>
#include <Qt>

#include "searchresultlist.h"
#include "utils.h"

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
    case Qt::Key_Alt:
      search_result_list->CurrentSearchResult()->SetTitle(alt_title_);
      break;
    case Qt::Key_Return:
      qDebug() << "CALLED";
      if (combination.keyboardModifiers() & Qt::AltModifier) {
        ProcessUrl(alt_url_, search_result_list);
        break;
      }

      ProcessUrl(url_, search_result_list);
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

defs::Action WebSearch::AltGo(const QString& arg) {
  return defs::Action::Nothing;
}

QString WebSearch::GetAltTitle() { return alt_title_; }

defs::Action WebSearch::Go(const QString& arg) { return defs::Action::Nothing; }

void WebSearch::Populate(const QJsonObject& object) {
  auto id_val = object["id"];
  auto id = id_val.toString();
  SetId(id);

  auto cmd_val = object["command"];
  auto cmd = cmd_val.toString();
  SetCommand(cmd);

  auto icon_val = object["icon"];
  auto icon = icon_val.toString();
  SetIcon(icon);

  auto url_val = object["url"];
  auto url = url_val.toString();
  url_ = url;

  auto title_val = object["title"];
  auto title = title_val.toString();
  SetTitle(title);

  auto placeholder_val = object["placeholder"];
  auto placeholder = placeholder_val.toString();
  SetPlaceholder(placeholder);

  auto description_val = object["description"];
  auto description = description_val.toString();
  SetDescription(description);

  auto alt_val = object["alt"];
  auto alt = alt_val.toObject();

  auto alt_url_val = alt["url"];
  auto alt_url = alt_url_val.toString();
  alt_url_ = alt_url;

  auto alt_title_val = alt["title"];
  auto alt_title = alt_title_val.toString();
  alt_title_ = alt_title;
}

void WebSearch::ProcessUrl(const QString& url,
                           SearchResultList* search_result_list) const {
  if (!url.contains("{}")) {
    search_result_list->HideParent();
    QDesktopServices::openUrl(QUrl(url));
  }

  auto arg = search_result_list->GetArg();

  // Means arg is equal to: QString().
  if (arg.isNull()) {
    emit search_result_list->TextReceived(GetCommand(true));
  }

  // Means arg is equal to: QString("").
  if (arg.isEmpty()) {
    return;
  }

  search_result_list->HideParent();
  QDesktopServices::openUrl(QUrl(utils::Format(url_, arg)));
}
