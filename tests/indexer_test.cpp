#include "indexer_test.h"

#include <QFile>
#include <QIODeviceBase>
#include <QJsonDocument>
#include <QTest>
#include <memory>
#include <utility>

#include "INIReader.h"
#include "core/indexer.h"
#include "core/models/applicationmodel.h"
#include "core/models/websearchmodel.h"

std::filesystem::path TestIndexer::GetParentPath() const {
  // Passing "/proc/self/exe" to std::filesystem::canonical is Linux-specific.
  return std::filesystem::canonical("/proc/self/exe").parent_path();
}

void TestIndexer::DeleteModel() {
  // Setup
  auto path = GetParentPath() / "firefox.desktop";
  auto reader = INIReader(path);
  QVERIFY(!(reader.ParseError() < 0));
  auto model = std::make_unique<ApplicationModel>(path, reader);
  auto id = model->GetId();
  auto& indexer = Indexer::GetInstance();
  indexer.IndexModel(std::move(model));

  auto ids = indexer.GetIds("firefox");
  QVERIFY(ids.contains(id));
  QCOMPARE(ids.size(), 1);

  // Cleanup
  indexer.DeleteModel(id);
  ids = indexer.GetIds("firefox");
  QVERIFY(!ids.contains(id));
  QCOMPARE(ids.size(), 0);
}

void TestIndexer::GetIds_InputfulModel() {
  // Setup
  auto path = GetParentPath() / "firefox.desktop";
  auto model = std::make_unique<ApplicationModel>(path, INIReader(path));
  auto id = model->GetId();
  auto& indexer = Indexer::GetInstance();
  indexer.IndexModel(std::move(model));

  // Test
  QFETCH(QString, input);
  QFETCH(int, size);

  auto ids = indexer.GetIds(input);
  QCOMPARE(ids.size(), size);

  // Cleanup
  indexer.DeleteModel(id);
}

void TestIndexer::GetIds_InputfulModel_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<int>("size");

  QTest::newRow("input-without-space") << "firefox" << 1;
  QTest::newRow("input-with-space") << "firefox " << 0;
}

void TestIndexer::GetIds_InputlessModel() {
  // Setup
  auto path = GetParentPath() / "duckduckgo.json";
  auto file = QFile{path};
  file.open(QIODeviceBase::ReadOnly);
  auto document = QJsonDocument::fromJson(file.readAll());
  auto model = std::make_unique<WebSearchModel>(document.object());
  auto id = model->GetId();
  auto& indexer = Indexer::GetInstance();
  indexer.IndexModel(std::move(model));

  // Test
  QFETCH(QString, input);
  QFETCH(int, size);

  auto ids = indexer.GetIds(input);
  QCOMPARE(ids.size(), size);

  // Cleanup
  indexer.DeleteModel(id);
}

void TestIndexer::GetIds_InputlessModel_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<int>("size");

  QTest::newRow("input-without-space") << "duck" << 1;
  QTest::newRow("input-with-space") << "duck " << 1;
}

void TestIndexer::Clear() {
  // Setup
  auto file = QFile{GetParentPath() / "duckduckgo.json"};
  file.open(QIODeviceBase::ReadOnly);
  auto document = QJsonDocument::fromJson(file.readAll());
  auto& indexer = Indexer::GetInstance();
  indexer.IndexModel(std::make_unique<WebSearchModel>(document.object()));

  // Test
  indexer.Clear();
  auto ids = indexer.GetIds("duck");
  QVERIFY(ids.empty());
}

void TestIndexer::UpdateTrie() {
  // Setup
  QFETCH(QString, old_url);
  QFETCH(QString, new_url);

  auto file = QFile{GetParentPath() / "duckduckgo.json"};
  file.open(QIODeviceBase::ReadOnly);
  auto document = QJsonDocument::fromJson(file.readAll());
  auto model = std::make_unique<WebSearchModel>(document.object());
  auto id = model->GetId();
  model->SetUrl(old_url);
  auto& indexer = Indexer::GetInstance();
  indexer.IndexModel(std::move(model));

  // This must be done because `model` will be nullptr after calling std::move.
  auto ws = dynamic_cast<WebSearchModel*>(indexer.GetModel(id));

  // Test
  auto old_tokens = ws->Tokenize();
  ws->SetUrl(new_url);
  auto new_tokens = ws->Tokenize();
  indexer.UpdateTrie(id, old_tokens, new_tokens);
  auto ids = indexer.GetIds(ws->GetCommand());
  QVERIFY(ids.contains(id));

  // Cleanup
  indexer.DeleteModel(id);
}

void TestIndexer::UpdateTrie_data() {
  QTest::addColumn<QString>("old_url");
  QTest::addColumn<QString>("new_url");

  QTest::newRow("from-receives-input-to-not-receives-input")
    << "https://duckduckgo.com/%1" << "https://duckduckgo.com";
  QTest::newRow("from-not-receives-input-to-receives-input")
    << "https://duckduckgo.com" << "https://duckduckgo.com/%1";
}
