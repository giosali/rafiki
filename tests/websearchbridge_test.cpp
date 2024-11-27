#include "websearchbridge_test.h"

#include <qtestcase.h>

#include <memory>

#include "core/bridges/websearchbridge.h"
#include "core/models/websearchmodel.h"

TestWebSearchBridge::TestWebSearchBridge() {
  object_with_formattable_title_ =
    QJsonObject{{"id", 1},
                {"command", "foo"},
                {"icon", ""},
                {"url", "https://example.com/%1"},
                {"title", "Search Example for '%1'"},
                {"placeholder", "..."}};
  object_with_unformattable_title_ = QJsonObject{{"id", 1},
                                                 {"command", "foo"},
                                                 {"icon", ""},
                                                 {"url", "https://example.com"},
                                                 {"title", "Go to Example"},
                                                 {"placeholder", ""}};
}

void TestWebSearchBridge::ProcessInput_FormattableTitle() {
  auto model = std::make_unique<WebSearchModel>(
    TestWebSearchBridge::object_with_formattable_title_);
  auto bridge = WebSearchBridge{};

  QFETCH(QString, input);
  QFETCH(QString, title);

  auto objects = bridge.ProcessInput(model.get(), input);
  QCOMPARE(objects[0]->GetTitle(), title);
}

void TestWebSearchBridge::ProcessInput_FormattableTitle_data() {
  auto title = object_with_formattable_title_["title"].toString();
  auto placeholder = object_with_formattable_title_["placeholder"].toString();

  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("title");

  QTest::newRow("one-letter") << "f" << title.arg(placeholder);
  QTest::newRow("full-match") << "foo" << title.arg(placeholder);
  QTest::newRow("full-match-with-space") << "foo " << title.arg(placeholder);
  QTest::newRow("full-match-with-input") << "foo hello" << title.arg("hello");
}

void TestWebSearchBridge::ProcessInput_UnformattableTitle() {
  auto model = std::make_unique<WebSearchModel>(
    TestWebSearchBridge::object_with_unformattable_title_);
  auto bridge = WebSearchBridge{};

  QFETCH(QString, input);
  QFETCH(int, size);
  QFETCH(QString, title);

  auto objects = bridge.ProcessInput(model.get(), input);
  QCOMPARE(size, objects.size());

  if (size != 0) {
    QCOMPARE(objects[0]->GetTitle(), title);
  }
}

void TestWebSearchBridge::ProcessInput_UnformattableTitle_data() {
  auto title = object_with_unformattable_title_["title"].toString();

  QTest::addColumn<QString>("input");
  QTest::addColumn<int>("size");
  QTest::addColumn<QString>("title");

  QTest::newRow("empty-input") << "" << 0 << "";
  QTest::newRow("one-letter") << "f" << 1 << title;
  QTest::newRow("full-match") << "foo" << 1 << title;
}
