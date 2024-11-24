#include <QCoreApplication>
#include <QObject>
#include <QTest>

#include "core/bridges/calculatorbridge_test.h"
#include "core/bridges/websearchbridge_test.h"

int main(int argc, char* argv[]) {
  // Absolutely necessary for tests to run.
  auto app = QCoreApplication{argc, argv};

  int status = 0;
  auto run_test = [&status, argc, argv](QObject* obj) {
    status |= QTest::qExec(obj, argc, argv);
  };

  run_test(new TestCalculatorBridge);
  run_test(new TestWebSearchBridge);

  return status;
}
