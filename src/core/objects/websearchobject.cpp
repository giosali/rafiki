#include "websearchobject.h"

#include <QDesktopServices>
#include <QUrl>

#include "../models/websearchmodel.h"

WebSearchObject::WebSearchObject(const FeatureModel* model,
                                 const QString& input, const QString& argument)
    : FeatureObject{model, input, argument}, argument_{argument} {}

void WebSearchObject::Drag() {}

void WebSearchObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      FeatureObject::ProcessKeyPress(combination);
      break;
    case Qt::Key_Return: {
      auto model = dynamic_cast<const WebSearchModel*>(GetModel());
      if (model == nullptr) {
        break;
      }

      auto url = combination.keyboardModifiers() & Qt::AltModifier
                   ? model->GetAltUrl()
                   : model->GetUrl();
      if (!url.contains("%1")) {
        emit Hidden();
        QDesktopServices::openUrl(QUrl{url});
        break;
      }

      // Means arg is equal to: QString{}.
      if (argument_.isNull()) {
        emit NewSearchBoxTextRequested(model->FormatCommand());
        break;
      }

      // Means arg is equal to: QString{""}.
      if (!argument_.isEmpty()) {
        emit Hidden();
        QDesktopServices::openUrl(QUrl{url.arg(argument_)});
      }

      break;
    }
    case Qt::Key_Alt:
      if (auto alt_title = GetAltTitle(); !alt_title.isEmpty()) {
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
