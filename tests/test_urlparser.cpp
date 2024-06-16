#include <QString>
#include <tuple>
#include <utility>

#include "../src/core/urlparser.h"
#include "utest.h"

UTEST(Url, is_valid) {
  auto pairs = std::vector<std::pair<QString, QString>>{
    {"www.co", "https://www.co"},
    {"example.co", "https://example.co"},
    {"example.com", "https://example.com"},
    {"example.com/", "https://example.com/"},
    {"example.com/example", "https://example.com/example"},

    {"example.com/showOrder.php?order=4621047",
     "https://example.com/showOrder.php?order=4621047"},
    {"example.com/companyInfo?name=C&H Sugar",
     "https://example.com/companyInfo?name=C&H Sugar"},
    {"sub.example.com/companyInfo?name=C&H Sugar",
     "https://sub.example.com/companyInfo?name=C&H Sugar"},
    {"sub.example.com/showCompanyInfo?name=C%26H%20Sugar",
     "https://sub.example.com/showCompanyInfo?name=C%26H%20Sugar"},

    {"example.com/index.html", "https://example.com/index.html"},

    {"www.example.com", "https://www.example.com"},
    {"example.co.uk", "https://example.co.uk"},

    {" https://example.com ", "https://example.com"},

    {"localhost", "https://localhost"},
    {"localhost/", "https://localhost/"},
    {"http://localhost", "http://localhost"},
    {"localhost:80", "https://localhost:80"},
    {"localhost:80/", "https://localhost:80/"},
    {"http://localhost:80", "http://localhost:80"},

    {"example.kcdncjksdjkkcndskjcnds",
     "https://example.kcdncjksdjkkcndskjcnds"},

    {"https://subdomain.domain.com:80/path/to/something?example=example",
     "https://subdomain.domain.com:80/path/to/something?example=example"},

    {"https://examp-le2.com", "https://examp-le2.com"},

    {"://example.com", "https://example.com"},
  };
  for (const auto& pair : pairs) {
    auto url = pair.first;
    auto expected = pair.second;

    auto parser = UrlParser{url};
    auto success = parser.IsValid();
    auto actual = parser.Url();

    auto url_c_str = ("url = " + url.toStdString()).c_str();
    // auto actual_c_str = ("actual = " + actual.toStdString()).c_str();
    auto actual_c_str = actual.toStdString().c_str();
    EXPECT_TRUE_MSG(success, url_c_str);
    EXPECT_TRUE_MSG(expected == actual, actual_c_str);
  }
}

UTEST(Url, is_invalid) {
  auto urls = std::vector<QString>{
    "example",
    "example.c",

    "hello world",

    "e$ample.com",
    "https2://example.com",
    "htt-ps://example.com",

    "exa mple.com",

    "http:example.com",
    "http:/example.com",

    "https://",
    "://",
  };
  for (const auto& url : urls) {
    auto parser = UrlParser{url};
    auto success = parser.IsValid();

    auto url_c_str = url.toStdString().c_str();
    EXPECT_FALSE_MSG(success, url_c_str);
  }
}
