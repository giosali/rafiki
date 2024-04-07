#ifndef SHUNTINGYARDALGORITHM_H
#define SHUNTINGYARDALGORITHM_H

#include <optional>
#include <queue>
#include <string>

class ShuntingYardAlgorithm {
 public:
  static std::optional<std::string> TryParse(const std::string& input);

 private:
  static int FindSurplus(double d);
  static bool IsLeftAssociative(char token);
  static bool IsNumber(char token);
  static std::optional<std::queue<std::string>> ParseInfixExpression(
    const std::string& expression, const std::string& decimal_separator);
  static std::string ParsePostfixExpression(
    std::queue<std::string>& expression);
  static int Precedence(char op);
  static std::string TrimZeros(const std::string& str);
};

#endif  // SHUNTINGYARDALGORITHM_H
