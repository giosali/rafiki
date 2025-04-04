#pragma once

#include <optional>
#include <queue>
#include <string>

#include "featurebridge.h"

class CalculatorBridge final : public FeatureBridge {
 public:
  std::vector<FeatureObject*> ProcessInput(FeatureModel* feature_model,
                                           const QString& input) override;

 private:
  class ShuntingYardAlgorithm {
   public:
    static std::optional<std::string> TryParse(const std::string& input);

   private:
    static int FindSurplus(double d);
    static bool IsLeftAssociative(char token);
    static bool IsNumber(char token);
    static std::optional<std::queue<std::string>> ParseInfixExpression(
      const std::string& expression);
    static std::string ParsePostfixExpression(std::queue<std::string>& queue);
    static int Precedence(char op);
    static std::string TrimZeros(const std::string& str);
  };
};
