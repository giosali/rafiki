#pragma once

#include "featurebridge.h"

class TrashBridge final : public FeatureBridge {
 public:
  std::vector<FeatureObject*> ProcessInput(const FeatureModel* feature_model,
                                           const QString& input) override;
};
