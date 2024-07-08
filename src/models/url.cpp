#include "url.h"

#include <QDesktopServices>

#include "../core/config.h"
#include "../core/io.h"
#include "../core/urlparser.h"

Url::Url() {
  SetDescription("Open URL");
  SetIcon(Io::GetFilePath(Io::Image::kUrl));
  SetId(Config::kApplicationAuthorId, 19);
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
    case Qt::Key_Return: {
      emit Hidden();
      QDesktopServices::openUrl(QUrl(GetTitle()));
      break;
    }
  }
}

void Url::ProcessKeyRelease(const QKeyCombination& combination,
                            const Input& input) {}
