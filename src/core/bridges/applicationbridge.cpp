#include "applicationbridge.h"

#include "../models/applicationmodel.h"
#include "../objects/applicationobject.h"

std::vector<std::shared_ptr<FeatureObject>> ApplicationBridge::ProcessInput(
  const FeatureModel* feature_model, const QString& input) {
  auto objects = std::vector<std::shared_ptr<FeatureObject>>{};
  auto model = static_cast<const ApplicationModel*>(feature_model);
  objects.push_back(std::make_shared<ApplicationObject>(model));
  return objects;
}
