#include "calculator.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QLocale>
#include <Qt>

#include "../core/shuntingyardalgorithm.h"

Calculator::Calculator()
    : ProcessedResult{
        kId,       kIcon,        kTitle,   kTitlePlaceholder,
        kAltTitle, kDescription, kCommand, kAppendSpaceToCommand} {}

bool Calculator::ProcessInput(const Input& input) {
  auto result = ShuntingYardAlgorithm::TryParse(input.GetFull().toStdString());
  if (!result.has_value()) {
    return false;
  }

  auto value = result.value();
  if (value.empty()) {
    kTitle = title_ = title_placeholder_;
    description_ = kInfoDescription;
  } else {
    kTitle = title_ = QString::fromStdString(value);
    description_ = kDescription;
  }

  return true;
}

void Calculator::Drag() {}

void Calculator::ProcessKeyPress(const QKeyCombination& combination,
                                 const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Return: {
      if (title_ == title_placeholder_) {
        break;
      }

      emit Hidden();
      auto clipboard = QGuiApplication::clipboard();
      clipboard->setText(title_);
      break;
    }
    case Qt::Key_Alt:
      emit NewTitleRequested(kTitle);
      break;
  }
}

void Calculator::ProcessKeyRelease(const QKeyCombination& combination,
                                   const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewTitleRequested(kTitle);
      break;
  }
}

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
