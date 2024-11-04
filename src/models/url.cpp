#include "url.h"

#include <QDesktopServices>
#include <QUrl>

#include "../core/paths.h"
#include "../core/urlparser.h"

Url::Url() {
  SetDescription("Open URL");
  SetIcon(Paths::Path(Paths::Image::kUrl));
  SetId(19);
}

bool Url::ProcessInput(const Input& input) {
  auto parser = UrlParser{input.ToString()};
  if (!parser.IsValid()) {
    return false;
  }

  SetTitle(parser.Url());
  return true;
}

void Url::Drag() {}

void Url::ProcessKeyPress(const QKeyCombination& combination,
                          const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Return:
      emit Hidden();
      QDesktopServices::openUrl(QUrl(GetTitle()));
      break;
    default:
      break;
  }
}

void Url::ProcessKeyRelease(const QKeyCombination& combination,
                            const Input& input) {}
