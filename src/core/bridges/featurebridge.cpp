#include "featurebridge.h"

std::pair<QString, QString> FeatureBridge::ParseInput(
  const QString& input, const QString& formatted_command,
  bool receives_input) const {
  // Due to the model being taken from its trie map container,
  // `formatted_command` will always begin with `input` when the length of
  // `input` is less than or equal to the length of `formatted_command`.
  if (input.length() <= formatted_command.length()) {
    return {input, {}};
  }

  // Checks commands that contain spaces and ignores models that are inputless.
  // when input is provided.
  if (!input.startsWith(formatted_command) ||
      (!receives_input && input.length() > formatted_command.length())) {
    return {};
  }

  // Returns input split into command and argument portions.
  auto argument = input.sliced(formatted_command.length());
  return {formatted_command, argument};
}
