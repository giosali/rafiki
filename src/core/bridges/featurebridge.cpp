#include "featurebridge.h"

std::pair<QString, QString> FeatureBridge::ParseInput(
  const QString& model_command, const QString& input,
  bool receives_input) const {
  // Due to the model being taken from its trie map container, `model_command`
  // will always begin with `input` when the length of `input` is less than or
  // equal to the length of `model_command`.
  if (input.length() <= model_command.length()) {
    return {input, {}};
  }

  // Checks commands that contain spaces and ignores models that are inputless.
  // when input is provided.
  if (!input.startsWith(model_command) ||
      (!receives_input && input.length() > model_command.length())) {
    return {};
  }

  // Returns input split into command and argument portions.
  auto index =
    input.indexOf(receives_input ? model_command + " " : model_command);
  return {input.sliced(0, index), input.sliced(index)};
}
