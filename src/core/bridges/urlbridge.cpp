#include "urlbridge.h"

#include <QFile>
#include <QIODeviceBase>
#include <QSet>
#include <QTextStream>
#include <cctype>

#include "../models/urlmodel.h"
#include "../objects/urlobject.h"

std::vector<FeatureObject*> UrlBridge::ProcessInput(FeatureModel* feature_model,
                                                    const QString& input) {
  auto parser = UrlParser{input};
  if (!parser.IsUrlValid()) {
    return {};
  }

  auto model = static_cast<UrlModel*>(feature_model);
  return {new UrlObject{model, parser.GetUrl()}};
}

UrlBridge::UrlParser::UrlParser(const QString& input) : url_{input.trimmed()} {
  Parse(url_);
}

QString UrlBridge::UrlParser::GetUrl() {
  return scheme_.isEmpty() ? kSchemeWithSeparator + url_ : url_;
}

bool UrlBridge::UrlParser::IsUrlValid() {
  // Hosts are required.
  if (host_.isEmpty()) {
    return false;
  }

  if (!IsHostValid()) {
    return false;
  }

  // Hosts don't need to contain a top-level domain if the host is `localhost`.
  if (top_level_domain_.isEmpty() && host_ != kLocalHost) {
    return false;
  }

  // Otherwise, the host needs to contain a top-level domain.
  if (host_ != kLocalHost && !IsTopLevelDomainValid()) {
    return false;
  }

  // Schemes are not required.
  // If there is a scheme, and the scheme is invalid, return false.
  if (!scheme_.isEmpty() && !IsSchemeValid()) {
    return false;
  }

  // Ports are not required.
  // If there is a port, and the port is invalid, return false.
  if (!port_.isEmpty() && !IsPortValid()) {
    return false;
  }

  return true;
}

const QString UrlBridge::UrlParser::kLocalHost{"localhost"};

const QString UrlBridge::UrlParser::kSchemeSeparator{"://"};

const QString UrlBridge::UrlParser::kSchemeWithSeparator{"https://"};

bool UrlBridge::UrlParser::IsHostValid() {
  for (size_t i = 0; i < host_.length(); ++i) {
    // Host can only consist of decimals, hyphens and alphanumeric characters.
    if (auto ch = host_[i]; ch.isLetterOrNumber() || ch == '.' || ch == '-') {
      continue;
    } else {
      return false;
    }
  }

  return true;
}

bool UrlBridge::UrlParser::IsPortValid() {
  // Ports aren't required.
  // Only numbers are allowed in ports.
  if (auto index = port_.indexOf(':'); index != -1) {
    for (size_t i = index + 1; i < port_.length(); ++i) {
      // Ports can only consist of numbers.
      if (!port_[i].isDigit()) {
        return false;
      }
    }
  }

  return true;
}

bool UrlBridge::UrlParser::IsSchemeValid() {
  for (size_t i = 0; i < scheme_.length(); ++i) {
    if (!scheme_[i].isLetter()) {
      return false;
    }
  }

  return true;
}

bool UrlBridge::UrlParser::IsTopLevelDomainValid() {
  auto file = QFile{":/data/tlds-alpha-by-domain.txt"};
  if (!file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
    return false;
  }

  auto top_level_domains = QSet<QString>{};
  for (auto stream = QTextStream{&file}; !stream.atEnd();) {
    top_level_domains.insert(stream.readLine().toLower());
  }

  return top_level_domains.contains(top_level_domain_.toLower());
}

void UrlBridge::UrlParser::Parse(const QString& url) {
  auto not_scheme = url;
  if (auto n = url.indexOf(kSchemeSeparator); n != -1) {
    scheme_ = url.sliced(0, n);
    auto separator_length = kSchemeSeparator.length();
    not_scheme = url.sliced(n + separator_length);

    // Removes "://" from "://example.com" if scheme is empty.
    // This is cleanup; maybe this should be done elsewhere.
    if (scheme_.isEmpty()) {
      url_ = url_.sliced(separator_length);
    }
  }

  if (auto n = not_scheme.indexOf('/'); n != -1) {
    // Excludes the path from the host (if there is one).
    host_ = not_scheme.sliced(0, n);
  } else {
    host_ = not_scheme;
  }

  // Separates the port from the host (if there is one).
  if (auto n = host_.lastIndexOf(':'); n != -1) {
    port_ = host_.sliced(n + 1);

    // This comes after since we're modifying the original `host_` property.
    host_ = host_.sliced(0, n);
  }

  // Looks for the top-level domain (if there is one).
  if (auto n = host_.lastIndexOf('.'); n != -1) {
    top_level_domain_ = host_.sliced(n + 1);
  }
}
