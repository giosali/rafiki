#include "calculator.h"

#include <QClipboard>
#include <QGuiApplication>

#include "searchresultlist.h"
#include "shuntingyardalgorithm.h"

Calculator::Calculator()
    : BaseResult{kId,          kIcon,    kTitle,      kAltTitle,
                 kDescription, kCommand, kPlaceholder} {}

QString Calculator::DragAndDrop() { return QString{}; }

bool Calculator::ProcessInput(const Input& input) {
  auto result = ShuntingYardAlgorithm::TryParse(input.GetFull().toStdString());
  if (!result.has_value()) {
    return false;
  }

  auto value = result.value();
  if (value.empty()) {
    title_ = placeholder_;
    description_ = kInfoDescription;
  } else {
    title_ = QString::fromStdString(value);
    description_ = kDescription;
  }

  return true;
}

void Calculator::ProcessKeyPress(const QKeyCombination& combination,
                                 QWidget* parent) {
  auto search_result_list = dynamic_cast<SearchResultList*>(parent);
  if (search_result_list == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Return:
      search_result_list->HideParent();
      if (title_ == placeholder_) {
        break;
      }

      auto clipboard = QGuiApplication::clipboard();
      clipboard->setText(title_);
      break;
  }
}

void Calculator::ProcessKeyRelease(const QKeyCombination& combination,
                                   QWidget* parent) {}
