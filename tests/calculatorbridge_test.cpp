#include "calculatorbridge_test.h"

TestCalculatorBridge::TestCalculatorBridge()
    : model_{std::make_unique<CalculatorModel>()} {}

void TestCalculatorBridge::ProcessInput_Addition() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Addition_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("one-digit") << "1 + 1" << "2";
  QTest::newRow("two-digit") << "11 + 11" << "22";
  QTest::newRow("multiple-numbers") << "1 + 1 + 1 + 1" << "4";
  QTest::newRow("negative-and-positive") << "-1 + 1" << "0";
  QTest::newRow("whole-and-fraction-decimal") << "1.1 + 1.1" << "2.2";
  QTest::newRow("only-fraction-decimal") << ".1 + .1" << "0.2";
}

void TestCalculatorBridge::ProcessInput_Decimals() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto objects = bridge_.ProcessInput(model_.get(), input);
  QVERIFY(!objects.empty());

  auto actual = objects[0]->GetTitle();
  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Decimals_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("whole-and-zero-fraction") << "1.0" << "1";
  QTest::newRow("whole-and-non-zero-fraction") << "1.1" << "1.1";
  QTest::newRow("zero-whole-and-non-zero-fraction") << "0.1" << "0.1";
  QTest::newRow("no-whole-and-non-zero-fraction") << ".1" << "0.1";
  QTest::newRow("no-whole-and-zero-fraction") << ".0" << "0";
  QTest::newRow("zero-whole-and-long-fraction")
    << "0.123456789" << "0.12345679";
  QTest::newRow("zero-whole-and-long-7-zero-left-padded-fraction")
    << "0.00000001" << "0.00000001";
  QTest::newRow("zero-whole-and-long-8-zero-left-padded-fraction")
    << "0.000000001" << "0";
}

void TestCalculatorBridge::ProcessInput_Division() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Division_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("basic") << "2 / 2" << "1";
  QTest::newRow("divide-by-zero") << "2 / 0" << "nan";
  QTest::newRow("negative-divide-by-zero") << "-2 / 0" << "nan";
  QTest::newRow("multiplication-then-division") << "3 * 3 / 3" << "3";
  QTest::newRow("division-then-multiplication") << "3 / 3 * 3" << "3";
  QTest::newRow("divisor-greater-than-dividend") << "1 / 3" << "0.33333333";
}

void TestCalculatorBridge::ProcessInput_Exponentiation() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Exponentiation_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("basic") << "2^2" << "4";
  QTest::newRow("basic-then-addition") << "2^2 + 2" << "6";
  QTest::newRow("exponent-parentheses") << "2^(2 + 2)" << "16";
  QTest::newRow("exponent-parentheses-then-addition")
    << "2^(2 + 2) + 2" << "18";
  QTest::newRow("decimal-exponent") << "2^2.2" << "4.59479342";
  QTest::newRow("decimal-base") << "2.2^2" << "4.84";
  QTest::newRow("double") << "2^2^2" << "16";
  QTest::newRow("zero-base") << "0^2" << "0";
  QTest::newRow("negative-base") << "-2^2" << "-4";
  QTest::newRow("double-negative-base") << "--2^2" << "4";
  QTest::newRow("parentheses-base") << "(1 + 1)^2" << "4";
  QTest::newRow("complex-1") << "(2 + 2^2)^3 / 5" << "43.2";
  QTest::newRow("complex-2") << "3 + 4 * 2 / ( 1 - 5 )^2^3" << "3.00012207";
}

void TestCalculatorBridge::ProcessInput_Factorial() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Factorial_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("basic") << "2!" << "2";
  QTest::newRow("negative") << "-2!" << "-2";
  QTest::newRow("large-number") << "100!" << "9.33262154e+157";
  QTest::newRow("addition") << "2 + 4!" << "26";
  QTest::newRow("addition-around") << "2 + 4! + 2" << "28";
  QTest::newRow("multiplication-then-addition") << "2 * 5! + 2" << "242";
  QTest::newRow("exponentiation-then-addition") << "2^2! + 2" << "6";
  QTest::newRow("decimal") << "2.2!" << "2.42396548";
  QTest::newRow("base-with-parentheses") << "(2)!" << "2";
  QTest::newRow("zero") << "0!" << "1";
  QTest::newRow("then-subtraction") << "2! - 1" << "1";
  QTest::newRow("complex") << "5 + 4! / 2! - 5" << "12";
}

void TestCalculatorBridge::ProcessInput_Invalid() {
  QFETCH(QString, input);

  auto objects = bridge_.ProcessInput(model_.get(), input);
  QVERIFY(objects.empty());
}

void TestCalculatorBridge::ProcessInput_Invalid_data() {
  QTest::addColumn<QString>("input");

  QTest::newRow("regular-string") << "hello-world";
  QTest::newRow("letter") << "2a + 2";
  QTest::newRow("less-than") << "0 < 2";
  QTest::newRow("reverse-factorial") << "!2";
  QTest::newRow("square-brackets") << "2 + [3 - 2]";
  QTest::newRow("curly-braces") << "2 + {3 - 2}";
  QTest::newRow("pipe") << "2 | 3";
  QTest::newRow("incomplete-addition") << "+2 - 1";
  QTest::newRow("incomplete-multiplication") << "*2";
  QTest::newRow("incomplete-division") << "/2";
  QTest::newRow("incomplete-exponentiation") << "^2";
  QTest::newRow("incomplete-parentheses") << ")2 + 2";
  QTest::newRow("double-addition") << "++2";
  QTest::newRow("exclamation-point") << "!";
  QTest::newRow("right-parenthesis") << ")";
}

void TestCalculatorBridge::ProcessInput_Incomplete() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Incomplete_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  auto placeholder = model_->GetTitlePlaceholder();

  QTest::newRow("example-1") << "(" << placeholder;
  QTest::newRow("example-2") << "(2 + 2" << placeholder;
  QTest::newRow("example-3") << "(2 + 2" << placeholder;
  QTest::newRow("example-4") << "2 + 2)" << placeholder;
  QTest::newRow("example-5") << "2 + 2))" << placeholder;
  QTest::newRow("example-6") << "(2 + 2))" << placeholder;
  QTest::newRow("example-7") << "((2 + 2)" << placeholder;
  QTest::newRow("example-8") << "2 ++ 2" << placeholder;
  QTest::newRow("example-9") << "2 // 2" << placeholder;
  QTest::newRow("example-10") << "2 ** 2" << placeholder;
  QTest::newRow("example-11") << "2 ^^ 2" << placeholder;
  QTest::newRow("example-12") << "2^" << placeholder;
  QTest::newRow("example-13") << "2^ " << placeholder;
  QTest::newRow("example-14") << "2^!" << placeholder;
  QTest::newRow("example-15") << "2^." << placeholder;
  QTest::newRow("example-16") << "2 + ." << placeholder;
  QTest::newRow("example-17") << "2 + ..." << placeholder;
  QTest::newRow("example-18") << "." << placeholder;
  QTest::newRow("example-19") << ".." << placeholder;
  QTest::newRow("example-20") << "2 -" << placeholder;
  QTest::newRow("example-21") << "2 +" << placeholder;
  QTest::newRow("example-22") << "2 /" << placeholder;
  QTest::newRow("example-23") << "2 *" << placeholder;
  QTest::newRow("example-24") << "2 ^" << placeholder;
}

void TestCalculatorBridge::ProcessInput_Multiplication() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Multiplication_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("basic") << "2 * 2" << "4";
  QTest::newRow("decimal") << "1.1 * 1.1" << "1.21";
  QTest::newRow("zero") << "0 * 2" << "0";
  QTest::newRow("negative") << "1 * -2" << "-2";
}

void TestCalculatorBridge::ProcessInput_Negatives() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Negatives_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("single-negative") << "-1" << "-1";
  QTest::newRow("double-negative") << "--1" << "1";
  QTest::newRow("triple-negative") << "---1" << "-1";
  QTest::newRow("quadruple-negative") << "----1" << "1";
  QTest::newRow("quintuple-negative") << "-----1" << "-1";
  QTest::newRow("zero") << "-0" << "0";
}

void TestCalculatorBridge::ProcessInput_Numbers() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Numbers_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("zero") << "0" << "0";
  QTest::newRow("one") << "1" << "1";
  QTest::newRow("1e15") << "1000000000000000" << "1000000000000000";
  QTest::newRow("1e16") << "10000000000000000" << "1e+16";
  QTest::newRow("before-positive-limit")
    << "10000000000000000000000000000000000000000000000000000000000000000000000"
       "00"
       "00000000000000000000000000000000000000000000000000000000000000000000000"
       "00"
       "00000000000000000000000000000000000000000000000000000000000000000000000"
       "00"
       "00000000000000000000000000000000000000000000000000000000000000000000000"
       "00"
       "00000000000000000"
    << "1e+308";
  QTest::newRow("before-negative-limit")
    << "-1000000000000000000000000000000000000000000000000000000000000000000000"
       "00"
       "0"
       "00000000000000000000000000000000000000000000000000000000000000000000000"
       "00"
       "00000000000000000000000000000000000000000000000000000000000000000000000"
       "00"
       "00000000000000000000000000000000000000000000000000000000000000000000000"
       "00"
       "00000000000000000"
    << "-1e+308";
  QTest::newRow("positive-limit") << "10000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000"
                                     "00000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000"
                                     "00000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000"
                                     "00000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000"
                                     "000000000000000000"
                                  << "∞";
  QTest::newRow("negative-limit") << "-1000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000"
                                     "0"
                                     "00000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000"
                                     "00000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000"
                                     "00000000000000000000000000000000000000000"
                                     "00000000000000000000000000000000"
                                     "000000000000000000"
                                  << "-∞";
}

void TestCalculatorBridge::ProcessInput_Parentheses() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Parentheses_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("single-number") << "(1)" << "1";
  QTest::newRow("single-decimal") << "(1.1)" << "1.1";
  QTest::newRow("multiple-parentheses") << "((((((1))))))" << "1";
  QTest::newRow("double-multiplication") << "(2)(2)" << "4";
  QTest::newRow("triple-multiplication") << "(2)(2)(2)" << "8";
  QTest::newRow("addition-inside-parentheses") << "(2 + 2)" << "4";
  QTest::newRow("addition-inside-parentheses-then-multiplication")
    << "2(2 + 2)" << "8";
  QTest::newRow("reverse-addition-inside-parentheses-then-multiplication")
    << "(2 + 2)2" << "8";
  QTest::newRow("miscellaneous-1") << "(5 * 4 + 3 * 2) - 1" << "25";
  QTest::newRow("miscellaneous-2") << "(2 - 2) + (4 + 4!) * 4 / (5)" << "22.4";
  QTest::newRow("factorial-within") << "(2 * 2!)" << "4";
  QTest::newRow("negative-result-inside-parentheses") << "2 + (1 - 3)" << "0";
  QTest::newRow("multiplication-with-expressions-1")
    << "(2 + 2)(2 + 2)" << "16";
  QTest::newRow("multiplication-with-expressions-2")
    << "(2 - 5)(4 / 2)" << "-6";
  QTest::newRow("no-whole-fraction-multiplication") << ".1(4)" << "0.4";
  QTest::newRow("reverse-no-whole-fraction-multiplication") << "(4).1" << "0.4";
  QTest::newRow("minus-sign") << "-(1 + 1)" << "-2";
  QTest::newRow("division-with-expressions")
    << "(2 - 1)/(2 + 1)" << "0.33333333";
}

void TestCalculatorBridge::ProcessInput_Pemdas() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Pemdas_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("example-1") << "36 * 12 + 3 / 5 - 2" << "430.6";
  QTest::newRow("example-2") << "2 * (3 * 2^2)" << "24";
  QTest::newRow("example-3") << "12 - ((8 + 7) * 2) / 6" << "7";
  QTest::newRow("example-4") << "24 - 3 * 4 / 2 + 6" << "24";
  QTest::newRow("example-5") << "6^2 - 5^2 + 1^2" << "12";
  QTest::newRow("example-6") << "2 + 8 / 2" << "6";
  QTest::newRow("example-7") << "20 / 5 * 2" << "8";
  QTest::newRow("example-8") << "2 + 2 * 2 + 2 / 2" << "7";
  QTest::newRow("example-9") << "12 - (2 * 4 + 1)" << "3";
  QTest::newRow("example-10") << "34 * 3 + 2 / 2 - (5 + 2)" << "96";
}

void TestCalculatorBridge::ProcessInput_Subtraction() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestCalculatorBridge::ProcessInput_Subtraction_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("basic") << "1 - 1" << "0";
  QTest::newRow("negative") << "-1 - 1" << "-2";
  QTest::newRow("double") << "1 - -1" << "2";
  QTest::newRow("triple") << "1 - --1" << "0";
  QTest::newRow("quadruple") << "1 - ---1" << "2";
  QTest::newRow("complex-expression-1") << "1 - (-1 - 1)" << "3";
  QTest::newRow("complex-expression-2") << "1 - -(-1 - 1)" << "-1";
  QTest::newRow("complex-expression-3") << "1 - -1 * 3" << "4";
  QTest::newRow("complex-expression-4") << "5 + -(2)(3 - 2 - 1 + 5)" << "-5";
  QTest::newRow("complex-expression-5") << "(3 - 2 - 1 + 5)" << "5";
  QTest::newRow("complex-expression-6") << "3 - 2 - 1 + 5" << "5";
  QTest::newRow("complex-expression-7") << "1 - 1 - 1 - 1" << "-2";
}
