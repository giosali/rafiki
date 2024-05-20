#include "websearch.h"

#include <QDesktopServices>
#include <QJsonValue>
#include <QUrl>
#include <Qt>

#include "../core/utils.h"
#include "../ui/mainwindow.h"

WebSearch::WebSearch(const QJsonObject& object)
    : BaseResult{object["id"].toString(),
                 object["icon"].toString(),
                 object["title"].toString(),
                 object["placeholder"].toString(),
                 object["alt"].toObject()["title"].toString(),
                 object["description"].toString(),
                 object["command"].toString(),
                 object["title"].toString().contains("{}")} {
  url_ = object["url"].toString();

  auto alt = object["alt"].toObject();
  alt_url_ = alt["url"].toString();
}

QString WebSearch::DragAndDrop() { return QString{}; }

void WebSearch::ProcessKeyPress(const QKeyCombination& combination) {
  auto main_window = MainWindow::Get();
  if (main_window == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Tab:
      if (auto command = FormatCommand();
          main_window->GetSearchBoxText().GetCmd() != command) {
        main_window->SetSearchBoxText(command);
      }

      break;
    case Qt::Key_Return: {
      auto url =
        combination.keyboardModifiers() & Qt::AltModifier ? alt_url_ : url_;
      if (!url.contains("{}")) {
        main_window->Hide();
        QDesktopServices::openUrl(QUrl(url));
        break;
      }

      auto arg = main_window->GetSearchBoxText().GetArg();

      // Means arg is equal to: QString().
      if (arg.isNull()) {
        main_window->SetSearchBoxText(FormatCommand());
        break;
      }

      // Means arg is equal to: QString("").
      if (arg.isEmpty()) {
        break;
      }

      main_window->Hide();
      QDesktopServices::openUrl(QUrl(utils::Format(url, arg)));
      break;
    }
    case Qt::Key_Alt:
      main_window->SetSearchResultTitle(alt_title_);
      break;
  }
}

void WebSearch::ProcessKeyRelease(const QKeyCombination& combination) {
  auto main_window = MainWindow::Get();
  if (main_window == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Alt:
      auto arg = main_window->GetSearchBoxText().GetArg();
      main_window->SetSearchResultTitle(FormatTitle(arg));
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
    emit search_result_list->TextReceived(FormatCommand());
    return;
  }

  // Means arg is equal to: QString("").
  if (arg.isEmpty()) {
    return;
  }

  search_result_list->HideParent();
  QDesktopServices::openUrl(QUrl(utils::Format(url_, arg)));
}
