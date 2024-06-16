#ifndef URLPARSER_H
#define URLPARSER_H

#include <QString>
#include <string>

class UrlParser {
 public:
  explicit UrlParser(const QString& url);

  bool IsValid();
  QString Url();

 private:
  static const std::string kLocalHost;
  static const std::string kSchemeSeparator;
  static const QString kSchemeWithSeparator;

  bool IsHostValid();
  bool IsPortValid();
  bool IsSchemeValid();
  bool IsTopLevelDomainValid();
  void Parse(const std::string& url);

  bool has_scheme_separator_;
  std::string host_;
  std::string port_;
  std::string scheme_;
  QString url_;
  std::string top_level_domain_;
};

#endif  // URLPARSER_H
