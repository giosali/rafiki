#include "websearchbridge.h"

#include "../models/websearchmodel.h"
#include "../objects/websearchobject.h"

std::vector<FeatureObject*> WebSearchBridge::PassInput(
  const FeatureModel* feature_model, const QString& input) const {
  auto model = static_cast<const WebSearchModel*>(feature_model);
  return {new WebSearchObject{model, input, input}};
}

std::vector<FeatureObject*> WebSearchBridge::ProcessInput(
  const FeatureModel* feature_model, const QString& input) {
  auto model = static_cast<const WebSearchModel*>(feature_model);
  auto pair = ParseInput(input, model->FormatCommand(), model->ReceivesInput());
  if (auto [command, argument] = pair;
      command.isEmpty() && argument.isEmpty()) {
    return {};
  } else {
    return {new WebSearchObject{model, command, argument}};
  }
}
