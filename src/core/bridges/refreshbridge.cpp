#include "refreshbridge.h"

#include "../models/refreshmodel.h"
#include "../objects/refreshobject.h"

std::vector<FeatureObject*> RefreshBridge::ProcessInput(
  FeatureModel* feature_model, const QString& input) {
  return {new RefreshObject{static_cast<RefreshModel*>(feature_model)}};
}
