#pragma once

#include "featurebridge.h"

class SettingsBridge : public FeatureBridge {
 public:
  std::vector<FeatureObject*> ProcessInput(FeatureModel* feature_model,
                                           const QString& input) override;
};
