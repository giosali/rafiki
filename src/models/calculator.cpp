#include "calculator.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QLocale>
#include <Qt>

#include "../core/io.h"
#include "../core/shuntingyardalgorithm.h"

Calculator::Calculator() : ProcessedResult{15} {
  SetDescription(kDescription);
  SetIcon(Io::GetFilePath(Io::ImageFile::kCalculator));
  SetTitlePlaceholder("...");
}

bool Calculator::ProcessInput(const Input& input) {
  auto result = ShuntingYardAlgorithm::TryParse(input.ToString().toStdString());
  if (!result.has_value()) {
    return false;
  }

  auto value = result.value();
  if (value.empty()) {
    SetTitle(TitlePlaceholder());
    SetDescription("Please enter a valid expression");
  } else {
    SetTitle(QString::fromStdString(value));
    SetDescription(kDescription);
  }

  return true;
}

void Calculator::Drag() {}

void Calculator::ProcessKeyPress(const QKeyCombination& combination,
                                 const Input& input) {
  auto title = Title();

  switch (combination.key()) {
    case Qt::Key_Return: {
      if (title == TitlePlaceholder()) {
        break;
      }

      emit Hidden();
      auto clipboard = QGuiApplication::clipboard();
      clipboard->setText(title);
      break;
    }
    case Qt::Key_Alt:
      emit NewTitleRequested(FormatNumber(title));
      break;
  }
}

void Calculator::ProcessKeyRelease(const QKeyCombination& combination,
                                   const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewTitleRequested(Title());
      break;
  }
}

const QString Calculator::kDescription{"Copy to clipboard"};

QString Calculator::FormatNumber(QString number) const {
  // Ignores values like "1e+15".
  if (number.contains('e')) {
    return number;
  }

  auto locale = QLocale::system();
  auto pos = number.contains(locale.decimalPoint())
               ? number.indexOf(locale.decimalPoint())
               : number.length();

  for (pos -= 3; pos > 0; pos -= 3) {
    number.insert(pos, locale.groupSeparator());
  }

  return number;
}
