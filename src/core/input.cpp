#include "input.h"

Input::Input(const QString& text) : text_{text} {
  // `space_index` increments in order to include the space when slicing.
  if (auto space_index = text.indexOf(" "); space_index++ != -1) {
    command_ = text.sliced(0, space_index);
    argument_ = text.sliced(space_index);
  } else {
    command_ = text;
  }
}

QString Input::Argument() const { return argument_; }

QString Input::Command() const { return command_; }

bool Input::IsCommandRedundant() const { return argument_.isEmpty(); }

bool Input::IsEmpty() const { return text_.isEmpty(); }

QString Input::ToString() const { return text_; }
