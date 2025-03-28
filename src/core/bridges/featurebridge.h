#pragma once

#include <QString>
#include <utility>
#include <vector>

#include "../objects/featureobject.h"

class FeatureModel;

class FeatureBridge {
 public:
  virtual std::vector<FeatureObject*> PassInput(FeatureModel* feature_model,
                                                const QString& input) const;
  virtual std::vector<FeatureObject*> ProcessInput(FeatureModel* feature_model,
                                                   const QString& input) = 0;

 protected:
  std::pair<QString, QString> ParseInput(
    const QString& input, const QString& formatted_command) const;
};
