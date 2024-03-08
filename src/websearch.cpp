#include "websearch.h"

#include <QDesktopServices>
#include <QJsonValue>
#include <QUrl>

#include "utils.h"

DataModel::Action WebSearch::AltGo(const QString& arg) {
  return ProcessUrl(alt_url_, arg);
}

QString WebSearch::GetAltTitle() { return alt_title_; }

DataModel::Action WebSearch::Go(const QString& arg) {
  return ProcessUrl(url_, arg);
}

void WebSearch::Populate(const QJsonObject& object) {
  auto id_val = object["id"];
  auto id = id_val.toInt();
  id_ = id;

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

DataModel::Action WebSearch::ProcessUrl(const QString& url,
                                        const QString& arg) const {
  if (!url.contains("{}")) {
    QDesktopServices::openUrl(QUrl(url));
    return Action::Nothing;
  }

  // Means that arg is equal to: QString().
  if (arg.isNull()) {
    return Action::SetTextToCommand;
  }

  // Means that arg is equal to: QString("").
  if (arg.isEmpty()) {
    return Action::Nothing;
  }

  QDesktopServices::openUrl(QUrl(utils::Format(url_, arg)));
  return Action::Nothing;
}
