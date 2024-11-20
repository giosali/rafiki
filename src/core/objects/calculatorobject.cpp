#include "calculatorobject.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QLocale>
#include <Qt>

CalculatorObject::CalculatorObject(const CalculatorModel* model,
                                   const std::string& value)
    : FeatureObject{model}, model_{model} {
  // Evaluates the result of the calculation.
  if (value.empty()) {
    SetTitle(model->GetTitlePlaceholder());
    SetDescription("Please enter a valid expression");
  } else {
    SetTitle(QString::fromStdString(value));
    SetDescription("Copy to clipboard");
  }
}

void CalculatorObject::Drag() {}

void CalculatorObject::ProcessKeyPress(const QKeyCombination& combination) {
  auto title = GetTitle();
  switch (combination.key()) {
    case Qt::Key_Return:
      if (title != model_->GetTitlePlaceholder()) {
        emit Hidden();
        auto clipboard = QGuiApplication::clipboard();
        clipboard->setText(title);
      }

      break;
    case Qt::Key_Alt:
      emit NewTitleRequested(FormatNumber(title));
      break;
    default:
      break;
  }
}

void CalculatorObject::ProcessKeyRelease(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewTitleRequested(GetTitle());
      break;
    default:
      break;
  }
}

QString CalculatorObject::FormatNumber(QString number) const {
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
