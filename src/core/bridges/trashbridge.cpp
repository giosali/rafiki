#include "trashbridge.h"

#include "../models/trashmodel.h"
#include "../objects/trashobject.h"

std::vector<std::shared_ptr<FeatureObject>> TrashBridge::ProcessInput(
  const FeatureModel* feature_model, const QString& input) {
  auto objects = std::vector<std::shared_ptr<FeatureObject>>{};
  auto model = static_cast<const TrashModel*>(feature_model);
  objects.push_back(std::make_shared<TrashObject>(model));
  return objects;
}
