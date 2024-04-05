#include "shuntingyardalgorithm.h"

#include <QLocale>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stack>
#include <stdexcept>

std::optional<std::string> ShuntingYardAlgorithm::TryParse(
  const std::string& input) {
  auto locale = QLocale{};
  auto postfix_expression =
    ParseInfixExpression(input, locale.decimalPoint().toStdString());
  if (!postfix_expression.has_value()) {
    return {};
  }

  return ParsePostfixExpression(postfix_expression.value());
}

int ShuntingYardAlgorithm::FindSurplus(double d) {
  // 8 == std::log10(1e8)
  int r = static_cast<int>(std::log10(d)) - 8;
  return r < 0 ? 0 : r;
}

bool ShuntingYardAlgorithm::IsNumber(char token) {
  switch (token) {
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return true;
    default:
      return false;
  }
}

std::optional<std::queue<std::string>>
ShuntingYardAlgorithm::ParseInfixExpression(
  const std::string& expression, const std::string& decimal_separator) {
  auto output = std::queue<std::string>{};
  auto operators = std::stack<char>{};
  auto buffer = std::string{};
  auto prev_token = '\0';

  // Checks if the buffer contains text first. Since the buffer is a number,
  // it must be pushed to the output before proceeding.
  auto flush_buffer = [&output, &buffer]() {
    if (buffer.empty()) {
      return;
    }

    output.push(buffer);
    buffer.clear();
  };

  for (size_t i = 0; i < expression.length(); ++i) {
    auto token = expression[i];
    switch (token) {
      case ' ':
        break;
      case '(':
        flush_buffer();

        // Handles multiplication through parentheses.
        // If the previous token is a number or a right parenthesis and the
        // current token is a left parenthesis, then that means that a
        // multiplication operator needs to be pushed to the operators stack.
        // This lets expressions like "2(2 + 2)" and "(2)(2)" be processed.
        if (IsNumber(prev_token) || prev_token == ')') {
          operators.push('*');
        }

        operators.push(token);
        break;
      case ')':
        // Exits if there are no operators.
        // This means the expression is invalid.
        if (operators.empty()) {
          return {};
        }

        flush_buffer();

        // Pops out all operators from the stack and pushes them to the output
        // until a left bracket is encountered.
        for (auto top = operators.top(); !operators.empty() && top != '(';
             operators.pop(), top = operators.top()) {
          output.push(std::string{top});
        }

        // Removes the left parenthesis '('.
        operators.pop();
        break;
      case '!':
      case '*':
      case '+':
      case '-':
      case '/':
      case '^':
        flush_buffer();

        // While the top of the stack of operators has greater precedence than
        // the current operator, pop them and push them to the queue.
        for (; !operators.empty(); operators.pop()) {
          auto top = operators.top();
          if (Precedence(top) <= Precedence(token)) {
            break;
          }

          output.push(std::string{top});
        }

        // If the current token is a minus sign and the operator at the
        // top of the stack of operators is also a minus sign, then
        // replace the top operator with a plus sign.
        // Otherwise, If the current token is a minus sign and the operator at
        // the top of the stack of operators is a plus sign, then replace the
        // top operator with a minus sign.
        if (!operators.empty() && token == '-') {
          auto top = operators.top();
          switch (top) {
            case '-':
            case '+':
              operators.pop();
              operators.push(',' * 2 - top);
              prev_token = token;
              continue;
          }
        }

        operators.push(token);
        break;
      case '.':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        // Handles multiplication through parentheses.
        // If the previous token is a right parenthesis and the current token is
        // a number, then that means that a multiplication operator needs to be
        // pushed to the operators stack.
        // This lets expressions like "(2 + 2)2" be processed.
        if (prev_token == ')') {
          operators.push('*');
        }

        buffer += token;
        break;
      default:
        return {};
    }

    prev_token = token;
  }

  // Make sure to always push numbers to the queue before pushing remaining
  // operators to the queue.
  flush_buffer();

  // Pushes remaining operators to queue.
  for (; !operators.empty(); operators.pop()) {
    output.push(std::string{operators.top()});
  }

  return output;
}

std::string ShuntingYardAlgorithm::ParsePostfixExpression(
  std::queue<std::string>& expression) {
  auto numbers = std::stack<double>{};
  for (; !expression.empty(); expression.pop()) {
    auto token = expression.front();

    // If the length of the string is greater than 1, then it is a number and
    // not an operator since operators will only consume one character.
    if (token.length() > 1) {
      try {
        numbers.push(std::stod(token));
      } catch (const std::out_of_range&) {
        numbers.push(std::numeric_limits<double>::infinity());
      }

      continue;
    }

    // At this point, it could be either a one-digit integer or an operator
    // since the token has a length of 1.
    auto ch = token[0];
    switch (ch) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        numbers.push(ch - '0');
        continue;
    }

    // At this point, the token can only be an operator.

    // If there are no numbers at this point (which means there's no right-side
    // operand), then something went wrong.
    if (numbers.empty()) {
      return "";
    }

    auto right_operand = numbers.top();
    switch (ch) {
      case '!':
        numbers.pop();
        numbers.push(std::tgamma(right_operand + 1));
        continue;
    }

    // We need to have at least two numbers in order to continue.
    if (numbers.size() == 1) {
      switch (ch) {
        case '-':
          numbers.pop();
          numbers.push(-right_operand);
          continue;
      }

      return "";
    }

    // At this point, there are at least two numbers in the stack which means we
    // can perform left and right side operations.
    // auto right_operand = numbers.top();
    numbers.pop();
    auto left_operand = numbers.top();
    numbers.pop();
    switch (ch) {
      case '*':
        numbers.push(left_operand * right_operand);
        break;
      case '+':
        numbers.push(left_operand + right_operand);
        break;
      case '-':
        numbers.push(left_operand - right_operand);
        break;
      case '/':
        if (right_operand == 0) {
          numbers.push(std::numeric_limits<double>::quiet_NaN());
          break;
        }

        numbers.push(left_operand / right_operand);
        break;
      case '^':
        numbers.push(std::pow(left_operand, right_operand));
        break;
    }
  }

  // There should only be one number at this point.
  if (numbers.size() != 1) {
    return "";
  }

  // Exits if the number is infinity with its string representation.
  auto top = numbers.top();
  if (std::isinf(top)) {
    return top < 0 ? "-∞" : "∞";
  }

  // Converting a double to a string using stringstream is favorable to
  // converting through std::to_string because the latter appends up to 6
  // zeros at the end.
  auto stream = std::ostringstream{};
  if (top > 1e15 || top < -1e15) {
    stream << std::setprecision(9) << top;
  } else {
    stream << std::fixed << std::setprecision(8 - FindSurplus(top)) << top;
  }

  return TrimZeros(stream.str());
}

int ShuntingYardAlgorithm::Precedence(char op) {
  switch (op) {
    case '!':
      return 4;
    case '^':
      return 3;
    case '*':
    case '/':
      return 2;
    case '+':
    case '-':
      return 1;
    default:
      return 0;
  }
}

std::string ShuntingYardAlgorithm::TrimZeros(const std::string& str) {
  auto dec_index = str.find('.');

  // Exits if there's no decimal point and if the last decimal place isn't a
  // '0'.
  if (dec_index == std::string::npos && str.back() != '0') {
    return str;
  }

  // The last not '0' index.
  auto ln_zero_idnex = str.find_last_not_of('0');

  // Trims numbers like "2.0" to "2" and "2.20" to "2.2".
  // If the last-not-zero index is equal to the decimal point index, the decimal
  // point needs to be removed as well.
  return str.substr(0, ln_zero_idnex + (dec_index == ln_zero_idnex ? 0 : 1));
}
