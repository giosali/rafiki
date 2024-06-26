#include "calculator.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QLocale>
#include <Qt>

#include "../core/io.h"
#include "../core/shuntingyardalgorithm.h"

Calculator::Calculator()
    : ProcessedResult{
        kId,       kIcon,        kTitle,   kTitlePlaceholder,
        kAltTitle, kDescription, kCommand, kAppendSpaceToCommand} {}

bool Calculator::ProcessInput(const Input& input) {
  auto result = ShuntingYardAlgorithm::TryParse(input.ToString().toStdString());
  if (!result.has_value()) {
    return false;
  }

  auto value = result.value();
  if (value.empty()) {
    title_ = title_placeholder_;
    description_ = kInfoDescription;
  } else {
    title_ = QString::fromStdString(value);
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
      emit NewTitleRequested(FormatNumber(title_));
      break;
  }
}

void Calculator::ProcessKeyRelease(const QKeyCombination& combination,
                                   const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewTitleRequested(title_);
      break;
  }
}

const QString Calculator::kAltTitle{};

const bool Calculator::kAppendSpaceToCommand{false};

const QString Calculator::kCommand{};

const QString Calculator::kDescription{"Copy to clipboard"};

const QString Calculator::kIcon{Io::GetIcon(Io::ImageFile::kCalculator)};

const uint64_t Calculator::kId{15};

const QString Calculator::kInfoDescription{"Please enter a valid expression"};

const QString Calculator::kTitle{};

const QString Calculator::kTitlePlaceholder{"..."};

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
