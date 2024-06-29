#include "url.h"

#include <QDesktopServices>

#include "../core/io.h"
#include "../core/urlparser.h"

Url::Url()
    : ProcessedResult{
        kId,       kIcon,        kTitle,   kTitlePlaceholder,
        kAltTitle, kDescription, kCommand, kAppendSpaceToCommand} {}

bool Url::ProcessInput(const Input& input) {
  auto parser = UrlParser{input.ToString()};
  if (!parser.IsValid()) {
    return false;
  }

  title_ = parser.Url();
  return true;
}

void Url::Drag() {}

void Url::ProcessKeyPress(const QKeyCombination& combination,
                          const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Return: {
      emit Hidden();
      QDesktopServices::openUrl(QUrl(title_));
      break;
    }
  }
}

void Url::ProcessKeyRelease(const QKeyCombination& combination,
                            const Input& input) {}

const QString Url::kAltTitle{};

const bool Url::kAppendSpaceToCommand{false};

const QString Url::kCommand{};

const QString Url::kDescription{"Open URL"};

const QString Url::kIcon{Io::GetFilePath(Io::ImageFile::kUrl)};

const uint64_t Url::kId{18};

const QString Url::kTitle{};

const QString Url::kTitlePlaceholder{};
