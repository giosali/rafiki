#include "trashbridge.h"

#include "../models/trashmodel.h"
#include "../objects/trashobject.h"

std::vector<FeatureObject*> TrashBridge::ProcessInput(
  const FeatureModel* feature_model, const QString& input) {
  auto model = static_cast<const TrashModel*>(feature_model);
  if (IsInputInvalid(input, model->GetCommand(), model->ReceivesInput())) {
    return {};
  } else {
    return {new TrashObject{model}};
  }
}
