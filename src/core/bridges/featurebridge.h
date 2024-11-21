#pragma once

#include <QString>
#include <utility>
#include <vector>

#include "../objects/featureobject.h"

class FeatureModel;

class FeatureBridge {
 public:
  virtual std::vector<FeatureObject*> ProcessInput(
    const FeatureModel* feature_model, const QString& input) = 0;

 protected:
  std::pair<QString, QString> ParseInput(const QString& model_command,
                                         const QString& input,
                                         bool receives_input) const;
};
