#include "urlbridge_test.h"

TestUrlBridge::TestUrlBridge() : model_{std::make_unique<UrlModel>()} {}

void TestUrlBridge::ProcessInput_Invalid() {
  QFETCH(QString, input);

  auto objects = bridge_.ProcessInput(model_.get(), input);
  QVERIFY(objects.empty());
}

void TestUrlBridge::ProcessInput_Invalid_data() {
  QTest::addColumn<QString>("input");

  QTest::newRow("only-domain") << "example";
  QTest::newRow("only-scheme") << "https://";
  QTest::newRow("only-scheme-colon-and-slashes") << "://";
  QTest::newRow("tld-shorter-than-2-characters") << "example.c";
  QTest::newRow("regular-string") << "hello world";
  QTest::newRow("dollar-sign-in-domain") << "e$ample.com";
  QTest::newRow("number-in-protocol") << "https2://example.com";
  QTest::newRow("hyphen-in-protocol") << "htt-ps://example.com";
  QTest::newRow("space-in-domain") << "exa mple.com";
  QTest::newRow("scheme-missing-two-slashes") << "http:example.com";
  QTest::newRow("scheme-missing-one-slash") << "http/:example.com";
}

void TestUrlBridge::ProcessInput_Valid() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto object = bridge_.ProcessInput(model_.get(), input)[0];
  auto actual = object->GetTitle();

  QCOMPARE(actual, expected);
}

void TestUrlBridge::ProcessInput_Valid_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("www-tld") << "www.co" << "https://www.co";
  QTest::newRow("domain-co") << "example.co" << "https://example.co";
  QTest::newRow("domain-com") << "example.com" << "https://example.com";
  QTest::newRow("domain-com-slash") << "example.com/" << "https://example.com/";
  QTest::newRow("domain-com-pathname")
    << "example.com/example" << "https://example.com/example";

  QTest::newRow("domain-tld-search-params")
    << "example.com/showOrder.php?order=4621047"
    << "https://example.com/showOrder.php?order=4621047";
  QTest::newRow("domain-tld-search-params-with-space")
    << "example.com/companyInfo?name=C&H Sugar"
    << "https://example.com/companyInfo?name=C&H Sugar";
  QTest::newRow("subdomain-domain-tld-search-params-with-space")
    << "sub.example.com/companyInfo?name=C&H Sugar"
    << "https://sub.example.com/companyInfo?name=C&H Sugar";
  QTest::newRow("subdomain-domain-tld-escaped-search-params-with-space")
    << "sub.example.com/showCompanyInfo?name=C%26H%20Sugar"
    << "https://sub.example.com/showCompanyInfo?name=C%26H%20Sugar";

  QTest::newRow("domain-com-period-in-pathname")
    << "example.com/index.html" << "https://example.com/index.html";

  QTest::newRow("subdomain-domain-tld")
    << "www.example.com" << "https://www.example.com";
  QTest::newRow("british-subdomain-domain-tld")
    << "example.co.uk" << "https://example.co.uk";

  QTest::newRow("surrounded-by-space")
    << " https://example.com " << "https://example.com";

  QTest::newRow("localhost") << "localhost" << "https://localhost";
  QTest::newRow("localhost-with-slash") << "localhost/" << "https://localhost/";
  QTest::newRow("localhost-with-scheme")
    << "http://localhost" << "http://localhost";
  QTest::newRow("localhost-with-port")
    << "localhost:80" << "https://localhost:80";
  QTest::newRow("localhost-with-port-and-slash")
    << "localhost:80/" << "https://localhost:80/";
  QTest::newRow("localhost-with-port-and-scheme")
    << "http://localhost:80" << "http://localhost:80";

  QTest::newRow("fake-tld") << "example.kcdncjksdjkkcndskjcnds"
                            << "https://example.kcdncjksdjkkcndskjcnds";

  QTest::newRow("composite-url")
    << "https://subdomain.domain.com:80/path/to/something?example=example"
    << "https://subdomain.domain.com:80/path/to/something?example=example";

  QTest::newRow("domain-with-hyphen") << "https://examp-le2.com"
                                      << "https://examp-le2.com";

  QTest::newRow("scheme-without-protocol") << "://example.com"
                                           << "https://example.com";
}
