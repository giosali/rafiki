#pragma once

#include "featurebridge.h"

class ApplicationBridge final : public FeatureBridge {
 public:
  std::vector<std::shared_ptr<FeatureObject>> ProcessInput(
    const FeatureModel* feature_model, const QString& input) override;
};
