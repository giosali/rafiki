#include "input.h"

Input::Input(const QString& text) : full_{text} {
  // Increments in order to include the space when slicing.
  if (auto space_index = text.indexOf(" "); space_index++ != -1) {
    cmd_ = text.sliced(0, space_index);
    arg_ = text.sliced(space_index);
    return;
  }

  cmd_ = text;
}

QString Input::GetArg() const { return arg_; }

QString Input::GetCmd() const { return cmd_; }

QString Input::GetFull() const { return full_; }

bool Input::IsEmpty() const { return full_.isEmpty(); }
