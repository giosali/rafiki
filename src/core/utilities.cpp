#include "utilities.h"

QString Utilities::RemoveAccents(const QString& input) {
  auto result = QString{};
  for (const QChar& ch : input) {
    auto decomposed = ch.decomposition();
    result += (decomposed.isEmpty() ? ch : decomposed[0]);
  }

  return result;
}

char Utilities::ToLower(char c) { return c < 'A' || c > 'Z' ? c : c + 32; }
