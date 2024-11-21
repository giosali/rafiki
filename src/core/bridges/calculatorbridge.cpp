#include "calculatorbridge.h"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stack>
#include <stdexcept>

#include "../models/calculatormodel.h"
#include "../objects/calculatorobject.h"

std::vector<FeatureObject*> CalculatorBridge::ProcessInput(
  const FeatureModel* feature_model, const QString& input) {
  auto result =
    CalculatorBridge::ShuntingYardAlgorithm::TryParse(input.toStdString());
  if (!result.has_value()) {
    return {};
  }

  auto model = static_cast<const CalculatorModel*>(feature_model);
  return {new CalculatorObject{model, result.value()}};
}

std::optional<std::string> CalculatorBridge::ShuntingYardAlgorithm::TryParse(
  const std::string& input) {
  auto postfix_queue = ParseInfixExpression(input);
  if (!postfix_queue.has_value()) {
    return std::nullopt;
  }

  return ParsePostfixExpression(postfix_queue.value());
}

int CalculatorBridge::ShuntingYardAlgorithm::FindSurplus(double d) {
  // 8 == std::log10(1e8)
  int r = static_cast<int>(std::log10(d)) - 8;
  return r < 0 ? 0 : r;
}

bool CalculatorBridge::ShuntingYardAlgorithm::IsLeftAssociative(char token) {
  switch (token) {
    case '!':
    case '^':
    case 'n':
      return false;
    case '*':
    case '+':
    case '-':
    case '/':
      return true;
    default:
      return false;
  }
}

bool CalculatorBridge::ShuntingYardAlgorithm::IsNumber(char token) {
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
CalculatorBridge::ShuntingYardAlgorithm::ParseInfixExpression(
  const std::string& expression) {
  // Returns nullopt if the first character is one of the following.
  switch (expression[0]) {
    case '!':
    case ')':
    case '*':
    case '+':
    case '/':
    case '^':
      return std::nullopt;
  }

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
        // Prevents previous token from being assigned to whitespace.
        continue;
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
      case ')': {
        flush_buffer();

        // Pops out all operators from the stack and pushes them to the output
        // until a left parenthesis is encountered. If a left parenthesis is
        // never encountered, a right parenthesis is pushed to the output.
        auto found_left_parenthesis = false;
        while (!operators.empty()) {
          auto top = operators.top();
          operators.pop();

          if (top == '(') {
            found_left_parenthesis = true;
            break;
          }

          output.push(std::string{top});
        }

        // Pushes the right parenthesis to the output queue if its corresponding
        // left parenthesis is never found.
        // This indicates that the postfix expression, when being parsed, will
        // fail.
        if (!found_left_parenthesis) {
          operators.push(')');
          // operators.push(std::string{")"});
        }

        break;
      }
      case '-':
        // Handles the unary '-' operator by checking if the previous token is
        // an operator is neither a right parenthesis nor an exclamation point.
        // The check for ')' makes expressions like "(1) - 1" possible.
        // Thec check for '!' makes expressions like "2! - 1" possible.
        if (!IsNumber(prev_token) && prev_token != ')' && prev_token != '!') {
          token = 'n';
        }

        [[fallthrough]];
      case '!':
      case '*':
      case '+':
      case '/':
      case '^':
        flush_buffer();

        // While the operator stack isn't empty, pop operators from the operator
        // stack and push them into the output queue if the stack operator has a
        // greater precedence than the token operator or if they have equal
        // precedence and the token operator is left-associative.
        while (!operators.empty()) {
          auto top = operators.top();
          auto stack_prec = Precedence(top);
          auto token_prec = Precedence(token);

          if (stack_prec > token_prec ||
              (stack_prec == token_prec && IsLeftAssociative(token))) {
            operators.pop();
            output.push(std::string{top});
            continue;
          }

          break;
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
        return std::nullopt;
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

std::string CalculatorBridge::ShuntingYardAlgorithm::ParsePostfixExpression(
  std::queue<std::string>& queue) {
  auto numbers = std::stack<double>{};
  for (; !queue.empty(); queue.pop()) {
    auto token = queue.front();

    // If the length of the string is greater than 1, then it is a number and
    // not an operator since operators will only consume one character.
    if (token.length() > 1) {
      try {
        numbers.push(std::stod(token));
      } catch (const std::out_of_range&) {
        numbers.push(std::numeric_limits<double>::infinity());
      } catch (const std::invalid_argument&) {
        return {};
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
      case ')':
        return {};
    }

    // At this point, the token can only be an operator.

    // If there are no numbers at this point (which means there's no right-side
    // operand), then something went wrong.
    if (numbers.empty()) {
      return {};
    }

    auto right_operand = numbers.top();
    switch (ch) {
      case '!':
        numbers.pop();
        numbers.push(std::tgamma(right_operand + 1));
        continue;
      case 'n':
        numbers.pop();

        // Prevents signed zeros.
        // '+ 0' prevents expressions like "-0" from evaluating to "-0".
        numbers.push(-right_operand + 0);
        continue;
    }

    // We need to have at least two numbers in order to continue.
    if (numbers.size() == 1) {
      return {};
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
        numbers.push(right_operand == 0
                       ? std::numeric_limits<double>::quiet_NaN()
                       : left_operand / right_operand);
        break;
      case '^':
        numbers.push(std::pow(left_operand, right_operand));
        break;
    }
  }

  // There should only be one number at this point.
  if (numbers.size() != 1) {
    return {};
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

int CalculatorBridge::ShuntingYardAlgorithm::Precedence(char op) {
  switch (op) {
    case '!':
      return 4;
    case '^':
    case 'n':
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

std::string CalculatorBridge::ShuntingYardAlgorithm::TrimZeros(
  const std::string& str) {
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
