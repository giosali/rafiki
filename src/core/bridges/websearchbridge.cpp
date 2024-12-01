#include "websearchbridge.h"

#include "../models/websearchmodel.h"
#include "../objects/websearchobject.h"

std::vector<FeatureObject*> WebSearchBridge::PassInput(
  FeatureModel* feature_model, const QString& input) const {
  auto model = static_cast<WebSearchModel*>(feature_model);
  return {new WebSearchObject{model, input, input}};
}

std::vector<FeatureObject*> WebSearchBridge::ProcessInput(
  FeatureModel* feature_model, const QString& input) {
  auto model = static_cast<WebSearchModel*>(feature_model);
  auto pair = ParseInput(input, model->FormatCommand());
  if (auto [command, argument] = pair;
      command.isEmpty() && argument.isEmpty()) {
    return {};
  } else {
    return {new WebSearchObject{model, command, argument}};
  }
}
