#include "websearchbridge.h"

#include "../models/websearchmodel.h"
#include "../objects/websearchobject.h"

std::vector<std::shared_ptr<FeatureObject>> WebSearchBridge::ProcessInput(
  const FeatureModel* feature_model, const QString& input) {
  auto objects = std::vector<std::shared_ptr<FeatureObject>>{};
  objects.reserve(1);

  auto model = static_cast<const WebSearchModel*>(feature_model);
  auto pair = ParseInput(model->GetCommand(), input, model->ReceivesInput());
  auto command = pair.first;
  auto argument = pair.second;
  if (command.isEmpty() && argument.isEmpty()) {
    return {};
  }

  objects.push_back(
    std::make_shared<WebSearchObject>(model, command, argument));
  return objects;
}
