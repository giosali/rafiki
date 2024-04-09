#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "../src/shuntingyardalgorithm.h"
#include "utest.h"

#define TEST_IOS(X)                                                      \
  do {                                                                   \
    for (const auto& io : X) {                                           \
      auto input = std::get<0>(io);                                      \
      auto expected = std::get<1>(io);                                   \
      auto actual = ShuntingYardAlgorithm::TryParse(input).value();      \
      EXPECT_STREQ_MSG(expected.c_str(), actual.c_str(), input.c_str()); \
    }                                                                    \
  } while (false)

UTEST(TryParse, addition) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"1 + 1", "2"});
  ios.push_back({"11 + 11", "22"});
  ios.push_back({"1.1 + 1.1", "2.2"});
  ios.push_back({".1 + .1", "0.2"});
  ios.push_back({"1 + 1 + 1 + 1", "4"});
  ios.push_back({"-1 + 1", "0"});
  TEST_IOS(ios);
}

UTEST(TryParse, decimals) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"1.0", "1"});
  ios.push_back({"1.1", "1.1"});
  ios.push_back({"0.1", "0.1"});
  ios.push_back({".1", "0.1"});
  ios.push_back({"0.123456789", "0.12345679"});
  ios.push_back({"0.00000001", "0.00000001"});
  ios.push_back({"0.000000001", "0"});
  ios.push_back({"(2).1", "0.2"});
  TEST_IOS(ios);
}

UTEST(TryParse, division) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"2 / 2", "1"});
  ios.push_back({"2 / 0", "nan"});
  ios.push_back({"-2 / 0", "nan"});
  ios.push_back({"3 * 3 / 3", "3"});
  ios.push_back({"3 / 3 * 3", "3"});
  ios.push_back({"1 / 3", "0.33333333"});
  TEST_IOS(ios);
}

UTEST(TryParse, exponentiation) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"2^2", "4"});
  ios.push_back({"2^2 + 2", "6"});
  ios.push_back({"2^(2 + 2) + 2", "18"});
  ios.push_back({"2^2.2", "4.59479342"});
  ios.push_back({"2.2^2", "4.84"});
  ios.push_back({"2^2^2", "16"});
  ios.push_back({"0^2", "0"});
  ios.push_back({"2^0", "1"});
  ios.push_back({"-2^2", "-4"});
  ios.push_back({"--2^2", "4"});
  ios.push_back({"(1 + 1)^2", "4"});
  ios.push_back({"(2 + 2^2)^3 / 5", "43.2"});
  ios.push_back({"3 + 4 * 2 / ( 1 - 5 )^2^3", "3.00012207"});
  TEST_IOS(ios);
}

UTEST(TryParse, factorial) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"2!", "2"});
  ios.push_back({"-2!", "-2"});
  ios.push_back({"100!", "9.33262154e+157"});
  ios.push_back({"2 + 4!", "26"});
  ios.push_back({"2 + 4! + 2", "28"});
  ios.push_back({"2 * 5! + 2", "242"});
  ios.push_back({"2^2! + 2", "6"});
  ios.push_back({"2.2!", "2.42396548"});
  ios.push_back({"(2)!", "2"});
  ios.push_back({"0!", "1"});
  ios.push_back({"5 + 4! / 2! - 5", "12"});
  ios.push_back({"2! - 1", "1"});
  TEST_IOS(ios);
}

UTEST(TryParse, multiplication) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"2 * 2", "4"});
  ios.push_back({"1.1 * 1.1", "1.21"});
  ios.push_back({"0 * 2", "0"});
  ios.push_back({"1 * -2", "-2"});
  TEST_IOS(ios);
}

UTEST(TryParse, negatives) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"-1", "-1"});
  ios.push_back({"--1", "1"});
  ios.push_back({"---1", "-1"});
  ios.push_back({"----1", "1"});
  ios.push_back({"-----1", "-1"});
  ios.push_back({"-0", "0"});
  TEST_IOS(ios);
}

UTEST(TryParse, numbers) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"0", "0"});
  ios.push_back({"1", "1"});
  ios.push_back({"1000000000000000", "1000000000000000"});
  ios.push_back({"10000000000000000", "1e+16"});
  ios.push_back(
    {"1000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "00000000000000000",
     "1e+308"});
  ios.push_back(
    {"-100000000000000000000000000000000000000000000000000000000000000000000000"
     "0"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "00000000000000000",
     "-1e+308"});
  ios.push_back(
    {"1000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "000000000000000000",
     "∞"});
  ios.push_back(
    {"-100000000000000000000000000000000000000000000000000000000000000000000000"
     "0"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "0000000000000000000000000000000000000000000000000000000000000000000000000"
     "000000000000000000",
     "-∞"});
  TEST_IOS(ios);
}

UTEST(TryParse, parentheses) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"(1)", "1"});
  ios.push_back({"((((((1))))))", "1"});
  ios.push_back({"(1.1)", "1.1"});
  ios.push_back({"(2)(2)", "4"});
  ios.push_back({"(2)(2)(2)", "8"});
  ios.push_back({"2(2 + 2)", "8"});
  ios.push_back({"(2 + 2)2", "8"});
  ios.push_back({"(2 + 2)", "4"});
  ios.push_back({"(5 * 4 + 3 * 2) - 1", "25"});
  ios.push_back({"(2 - 2) + (4 + 4!) * 4 / (5)", "22.4"});
  ios.push_back({"(2 * 2!)", "4"});
  ios.push_back({"2 + (1 - 3)", "0"});
  ios.push_back({"(2 + 2)(2 + 2)", "16"});
  ios.push_back({"(2 - 5)(4 / 2)", "-6"});
  ios.push_back({".1(4)", "0.4"});
  ios.push_back({"-(1 + 1)", "-2"});
  ios.push_back({"(2 - 1)/(2 + 1)", "0.33333333"});
  TEST_IOS(ios);
}

UTEST(TryParse, pemdas) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"36 * 12 + 3 / 5 - 2", "430.6"});
  ios.push_back({"2 * (3 * 2^2)", "24"});
  ios.push_back({"12 - ((8 + 7) * 2) / 6", "7"});
  ios.push_back({"24 - 3 * 4 / 2 + 6", "24"});
  ios.push_back({"6^2 - 5^2 + 1^2", "12"});
  ios.push_back({"2 + 8 / 2", "6"});
  ios.push_back({"20 / 5 * 2", "8"});
  ios.push_back({"2 + 2 * 2 + 2 / 2", "7"});
  ios.push_back({"12 - (2 * 4 + 1)", "3"});
  ios.push_back({"34 * 3 + 2 / 2 - (5 + 2)", "96"});
  TEST_IOS(ios);
}

UTEST(TryParse, returns_empty) {
  auto inputs = std::vector<std::string>{};
  inputs.push_back("(");
  inputs.push_back("(2 + 2");
  inputs.push_back("(2 + 2");
  inputs.push_back("2 + 2)");
  inputs.push_back("2 + 2))");
  inputs.push_back("(2 + 2))");
  inputs.push_back("((2 + 2)");
  inputs.push_back("2 ++ 2");
  inputs.push_back("2 // 2");
  inputs.push_back("2 ** 2");
  inputs.push_back("2^^2");
  inputs.push_back("2^");
  inputs.push_back("2^ ");
  inputs.push_back("2^!");
  inputs.push_back("2^.");
  inputs.push_back("2 + ...");
  inputs.push_back("2 + .");
  inputs.push_back(".");
  inputs.push_back("..");
  inputs.push_back("2 -");
  inputs.push_back("2 +");
  inputs.push_back("2 /");
  inputs.push_back("2 *");
  inputs.push_back("2 ^");
  for (const auto& input : inputs) {
    auto result = ShuntingYardAlgorithm::TryParse(input);
    EXPECT_STREQ_MSG("", result.value().c_str(), input.c_str());
  }
}

UTEST(TryParse, returns_invalid) {
  auto inputs = std::vector<std::string>{};
  inputs.push_back("hello world");
  inputs.push_back("2a + 2");
  inputs.push_back("0 < 2");
  inputs.push_back("!2");
  inputs.push_back("2 + [3 -2]");
  inputs.push_back("2 + {3 -2}");
  inputs.push_back("2 | 3");
  inputs.push_back("+2 - 1");
  inputs.push_back("++2");
  inputs.push_back("*2");
  inputs.push_back("/2");
  inputs.push_back("^2");
  inputs.push_back(")2 + 2");
  inputs.push_back("!");
  inputs.push_back(")");
  for (const auto& input : inputs) {
    auto result = ShuntingYardAlgorithm::TryParse(input);
    EXPECT_FALSE_MSG(result.has_value(), input.c_str());
  }
}

UTEST(TryParse, subtraction) {
  auto ios = std::vector<std::tuple<std::string, std::string>>{};
  ios.push_back({"1 - 1", "0"});
  ios.push_back({"-1 - 1", "-2"});
  ios.push_back({"1 - -1", "2"});
  ios.push_back({"1 - - -1", "0"});
  ios.push_back({"1 - - - -1", "2"});
  ios.push_back({"1 - (-1 - 1)", "3"});
  ios.push_back({"1 - -(-1 - 1)", "-1"});
  ios.push_back({"1 - -1 * 3", "4"});
  ios.push_back({"5 + -(2)(3 - 2 - 1 + 5)", "-5"});
  ios.push_back({"(3 - 2 - 1 + 5)", "5"});
  ios.push_back({"3 - 2 - 1 + 5", "5"});
  ios.push_back({"1 - 1 - 1 - 1", "-2"});
  TEST_IOS(ios);
}
