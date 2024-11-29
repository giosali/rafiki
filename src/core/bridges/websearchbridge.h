#pragma once

#include "featurebridge.h"

class WebSearchBridge final : public FeatureBridge {
 public:
  std::vector<FeatureObject*> PassInput(FeatureModel* feature_model,
                                        const QString& input) const override;
  std::vector<FeatureObject*> ProcessInput(FeatureModel* feature_model,
                                           const QString& input) override;
};
