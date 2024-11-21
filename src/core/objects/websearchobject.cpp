#include "websearchobject.h"

#include <QDesktopServices>
#include <QUrl>
#include <Qt>

WebSearchObject::WebSearchObject(const WebSearchModel* model,
                                 const QString& command,
                                 const QString& argument)
    : FeatureObject{model, argument},
      argument_{argument},
      command_{command},
      model_{model} {}

void WebSearchObject::Drag() {}

void WebSearchObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      if (command_ != model_->FormatCommand()) {
        emit NewSearchBoxTextRequested(command_);
      }

      break;
    case Qt::Key_Return: {
      auto url = combination.keyboardModifiers() & Qt::AltModifier
                   ? model_->GetAltUrl()
                   : model_->GetUrl();
      if (!url.contains("%1")) {
        emit Hidden();
        QDesktopServices::openUrl(QUrl{url});
        break;
      }

      // Means arg is equal to: QString{}.
      if (argument_.isNull()) {
        emit NewSearchBoxTextRequested(model_->FormatCommand());
        break;
      }

      // Means arg is equal to: QString{""}.
      if (argument_.isEmpty()) {
        break;
      }

      emit Hidden();
      QDesktopServices::openUrl(QUrl{url.arg(argument_)});
      break;
    }
    case Qt::Key_Alt:
      if (auto alt_title = GetAltTitle(); !alt_title.isNull()) {
        emit NewTitleRequested(alt_title);
      }

      break;
    default:
      break;
  }
}

void WebSearchObject::ProcessKeyRelease(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewTitleRequested(GetTitle());
      break;
    default:
      break;
  }
}
