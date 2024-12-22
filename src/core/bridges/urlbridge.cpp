#include "urlbridge.h"

#include <QFile>
#include <QIODeviceBase>
#include <QTextStream>
#include <cctype>

#include "../models/urlmodel.h"
#include "../objects/urlobject.h"

std::vector<FeatureObject*> UrlBridge::ProcessInput(FeatureModel* feature_model,
                                                    const QString& input) {
  parser_.Parse(input);
  if (!parser_.IsUrlValid()) {
    return {};
  }

  auto model = static_cast<UrlModel*>(feature_model);
  return {new UrlObject{model, parser_.GetUrl()}};
}

UrlBridge::UrlParser::UrlParser() {
  auto file = QFile{":/data/tlds-alpha-by-domain.txt"};
  if (!file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
    return;
  }

  for (auto stream = QTextStream{&file}; !stream.atEnd();) {
    tlds_.insert(stream.readLine().toLower().toStdString());
  }
}

bool UrlBridge::UrlParser::IsUrlValid() {
  // Hosts are required.
  if (host_.empty()) {
    return false;
  }

  if (!IsHostValid()) {
    return false;
  }

  // Hosts don't need to contain a top-level domain if the host is `localhost`.
  if (top_level_domain_.empty() && host_ != kLocalHost) {
    return false;
  }

  // Otherwise, the host needs to contain a top-level domain.
  if (host_ != kLocalHost && !IsTopLevelDomainValid()) {
    return false;
  }

  // Schemes are not required.
  // If there is a scheme, and the scheme is invalid, return false.
  if (!scheme_.empty() && !IsSchemeValid()) {
    return false;
  }

  // Ports are not required.
  // If there is a port, and the port is invalid, return false.
  if (!port_.empty() && !IsPortValid()) {
    return false;
  }

  return true;
}

void UrlBridge::UrlParser::Parse(const QString& input) {
  url_ = input.trimmed().toLower();

  auto url = input.toStdString();
  auto not_scheme = url;
  if (auto n = url.find(kSchemeSeparator); n != std::string::npos) {
    scheme_ = url.substr(0, n);
    not_scheme = url.substr(n + kSchemeSeparator.length());

    // Removes "://" from "://example.com" if scheme is empty.
    // This is cleanup; maybe this should be done elsewhere.
    if (scheme_.empty()) {
      url_ = url_.sliced(kSchemeSeparator.length());
    }
  }

  if (auto n = not_scheme.find('/'); n != std::string::npos) {
    // Excludes the path from the host (if there is one).
    host_ = not_scheme.substr(0, n);
  } else {
    host_ = not_scheme;
  }

  // Separates the port from the host (if there is one).
  if (auto n = host_.find_last_of(':'); n != std::string::npos) {
    port_ = host_.substr(n + 1);

    // This comes after since we're modifying the original `host_` property.
    host_ = host_.substr(0, n);
  }

  // Looks for the top-level domain (if there is one).
  if (auto n = host_.find_last_of('.'); n != std::string::npos) {
    top_level_domain_ = host_.substr(n + 1);
  }
}

QString UrlBridge::UrlParser::GetUrl() {
  return scheme_.empty() ? kSchemeWithSeparator + url_ : url_;
}

const std::string UrlBridge::UrlParser::kLocalHost{"localhost"};

const std::string UrlBridge::UrlParser::kSchemeSeparator{"://"};

const QString UrlBridge::UrlParser::kSchemeWithSeparator{"https://"};

bool UrlBridge::UrlParser::IsHostValid() {
  for (size_t i = 0; i < host_.length(); ++i) {
    // From: https://en.cppreference.com/w/cpp/string/byte/isalpha
    // "Like all other functions from <cctype>, the behavior of std::isalpha is
    // undefined if the argument's value is neither representable as unsigned
    // char nor equal to EOF. To use these functions safely with plain chars (or
    // signed chars), the argument should first be converted to unsigned char."
    auto ch = static_cast<unsigned char>(host_[i]);

    // Host can only consist of decimals, hyphens and alphanumeric characters.
    if (std::isalnum(ch) || ch == '.' || ch == '-') {
      continue;
    }

    return false;
  }

  return true;
}

bool UrlBridge::UrlParser::IsPortValid() {
  // Ports aren't required.
  // Only numbers are allowed in ports.
  if (auto index = port_.find(':'); index != std::string::npos) {
    for (size_t i = index + 1; i < port_.length(); ++i) {
      // Ports can only consist of numbers.
      if (!std::isdigit(static_cast<unsigned char>(port_[i]))) {
        return false;
      }
    }
  }

  return true;
}

bool UrlBridge::UrlParser::IsSchemeValid() {
  for (size_t i = 0; i < scheme_.length(); ++i) {
    if (!std::isalpha(static_cast<unsigned char>(scheme_[i]))) {
      return false;
    }
  }

  return true;
}

bool UrlBridge::UrlParser::IsTopLevelDomainValid() {
  return tlds_.contains(top_level_domain_);
}
