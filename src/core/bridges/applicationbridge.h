#pragma once

#include "featurebridge.h"

class ApplicationBridge final : public FeatureBridge {
 public:
  std::vector<FeatureObject*> ProcessInput(FeatureModel* feature_model,
                                           const QString& input) override;
};
