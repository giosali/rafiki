#include <QCoreApplication>
#include <QObject>
#include <QTest>

#include "core/bridges/calculatorbridge_test.h"
#include "core/bridges/urlbridge_test.h"
#include "core/bridges/websearchbridge_test.h"
#include "include/tsl/htrie_map_test.h"

int main(int argc, char* argv[]) {
  // Absolutely necessary for tests to run.
  auto app = QCoreApplication{argc, argv};

  int status = 0;
  auto run_test = [&status, argc, argv](QObject* obj) {
    status |= QTest::qExec(obj, argc, argv);
  };

  run_test(new TestCalculatorBridge);
  run_test(new TestUrlBridge);
  run_test(new TestWebSearchBridge);
  run_test(new TestHtrieMap);

  return status;
}
