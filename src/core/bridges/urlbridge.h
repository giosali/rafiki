#pragma once

#include <QString>
#include <string>

#include "featurebridge.h"

class UrlBridge final : public FeatureBridge {
 public:
  std::vector<FeatureObject*> ProcessInput(FeatureModel* feature_model,
                                           const QString& input) override;

 private:
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
};
