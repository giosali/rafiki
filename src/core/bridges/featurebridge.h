#pragma once

#include <QString>
#include <memory>
#include <utility>
#include <vector>

#include "../objects/featureobject.h"

class FeatureModel;

class FeatureBridge {
 public:
  virtual std::vector<std::shared_ptr<FeatureObject>> ProcessInput(
    const FeatureModel* feature_model, const QString& input) = 0;

 protected:
  std::pair<QString, QString> ParseInput(const QString& model_command,
                                         const QString& input,
                                         bool receives_input) const;
};
