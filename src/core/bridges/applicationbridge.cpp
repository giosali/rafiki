#include "applicationbridge.h"

#include "../models/applicationmodel.h"
#include "../objects/applicationobject.h"

std::vector<FeatureObject*> ApplicationBridge::ProcessInput(
  FeatureModel* feature_model, const QString& input) {
  auto model = static_cast<ApplicationModel*>(feature_model);
  return {new ApplicationObject{model}};
}
