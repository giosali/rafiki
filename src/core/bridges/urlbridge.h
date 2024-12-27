#pragma once

#include <QString>

#include "featurebridge.h"

class UrlBridge final : public FeatureBridge {
 public:
  std::vector<FeatureObject*> ProcessInput(FeatureModel* feature_model,
                                           const QString& input) override;

 private:
  class UrlParser {
   public:
    explicit UrlParser(const QString& input);

    QString GetUrl();
    bool IsUrlValid();

   private:
    static const QString kLocalHost;
    static const QString kSchemeSeparator;
    static const QString kSchemeWithSeparator;

    bool IsHostValid();
    bool IsPortValid();
    bool IsSchemeValid();
    bool IsTopLevelDomainValid();
    void Parse(const QString& url);

    bool has_scheme_separator_;
    QString host_;
    QString port_;
    QString scheme_;
    QString top_level_domain_;
    QString url_;
  };
};
