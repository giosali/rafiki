#include "applicationbridge.h"

#include "../models/applicationmodel.h"
#include "../objects/applicationobject.h"

std::vector<FeatureObject*> ApplicationBridge::ProcessInput(
  const FeatureModel* feature_model, const QString& input) {
  auto model = static_cast<const ApplicationModel*>(feature_model);
  if (IsInputInvalid(input, model->GetCommand(), model->ReceivesInput())) {
    return {};
  } else {
    return {new ApplicationObject{model}};
  }
}
