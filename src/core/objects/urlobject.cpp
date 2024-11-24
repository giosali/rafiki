#include "urlobject.h"

#include <QDesktopServices>
#include <QUrl>

UrlObject::UrlObject(const UrlModel* model, const QString& url)
    : FeatureObject{model}, model_{model} {
  SetTitle(url);
}

void UrlObject::Drag() {}

void UrlObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Return:
      emit Hidden();
      QDesktopServices::openUrl(QUrl(GetTitle()));
      break;
    default:
      break;
  }
}

void UrlObject::ProcessKeyRelease(const QKeyCombination& combination) {}
