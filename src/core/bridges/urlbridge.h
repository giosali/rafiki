#pragma once

#include <QString>
#include <string>
#include <unordered_set>

#include "featurebridge.h"

class UrlBridge final : public FeatureBridge {
 public:
  std::vector<FeatureObject*> ProcessInput(FeatureModel* feature_model,
                                           const QString& input) override;

 private:
  class UrlParser {
   public:
    UrlParser();

    QString GetUrl();
    bool IsUrlValid();
    void Parse(const QString& input);

   private:
    static const std::string kLocalHost;
    static const std::string kSchemeSeparator;
    static const QString kSchemeWithSeparator;

    bool IsHostValid();
    bool IsPortValid();
    bool IsSchemeValid();
    bool IsTopLevelDomainValid();

    bool has_scheme_separator_;
    std::string host_;
    std::string port_;
    std::string scheme_;
    QString url_;
    std::unordered_set<std::string> tlds_{};
    std::string top_level_domain_;
  };

  UrlParser parser_{};
};
