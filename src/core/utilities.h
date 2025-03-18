#pragma once

#include <QString>

class Utilities {
 public:
  Utilities() = delete;

  static QString RemoveAccents(const QString& input);
  static char ToLower(char c);
};
