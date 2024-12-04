#include <QCoreApplication>
#include <QObject>
#include <QTest>

#include "calculatorbridge_test.h"
#include "htrie_map_test.h"
#include "indexer_test.h"
#include "urlbridge_test.h"
#include "utilities_test.h"
#include "websearchbridge_test.h"

int main(int argc, char* argv[]) {
  // Absolutely necessary for tests to run.
  auto app = QCoreApplication{argc, argv};

  int status = 0;
  auto run_test = [&status, argc, argv](QObject* obj) {
    status |= QTest::qExec(obj, argc, argv);
  };

  run_test(new TestCalculatorBridge);
  run_test(new TestHtrieMap);
  run_test(new TestIndexer);
  run_test(new TestUrlBridge);
  run_test(new TestUtilities);
  run_test(new TestWebSearchBridge);

  return status;
}
