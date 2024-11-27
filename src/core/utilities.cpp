#include "utilities.h"

#include <QDir>

QString Utilities::Combine(const QString& p1, const QString& p2) {
  return QDir::cleanPath(p1 + QDir::separator() + p2);
}

std::vector<QString> Utilities::Split(const QString& str,
                                      const QChar& delimiter, int maxsplit) {
  auto tokens = std::vector<QString>{};
  auto start = size_t{0};
  auto end = size_t{0};
  auto splits = 0;

  while (start < str.length()) {
    // If we've reached maxsplit, add the rest of the string as the last token
    if (maxsplit >= 0 && splits >= maxsplit) {
      tokens.push_back(str.mid(start));
      break;
    }

    // Find next delimiter
    end = str.indexOf(delimiter, start);

    // If no more delimiters found, add the rest as the last token
    if (end == -1) {
      if (start < str.length()) {
        tokens.push_back(str.mid(start));
      }
      break;
    }

    // Add token to result
    if (end - start > 0) {
      tokens.push_back(str.mid(start, end - start));
    } else if (end == start) {
      // Handle empty token between consecutive delimiters
      tokens.push_back(QString());
    }

    // Move past the delimiter
    start = end + 1;
    ++splits;
  }

  return tokens;
}

std::vector<std::string> Utilities::Split(const std::string& str,
                                          char delimiter, int maxsplit) {
  auto tokens = std::vector<std::string>{};
  auto start = size_t{0};
  auto end = size_t{0};
  auto splits = 0;

  while (start < str.length()) {
    // If we've reached maxsplit, add the rest of the string as the last token
    if (maxsplit >= 0 && splits >= maxsplit) {
      tokens.push_back(str.substr(start));
      break;
    }

    // Find next delimiter
    end = str.find(delimiter, start);

    // If no more delimiters found, add the rest as the last token
    if (end == std::string::npos) {
      if (start < str.length()) {
        tokens.push_back(str.substr(start));
      }

      break;
    }

    // Add token to result
    if (end - start > 0) {
      tokens.push_back(str.substr(start, end - start));
    } else if (end == start) {
      // Handle empty token between consecutive delimiters
      tokens.push_back("");
    }

    // Move past the delimiter
    start = end + 1;
    ++splits;
  }

  return tokens;
}

char Utilities::ToLower(char c) { return c < 'A' || c > 'Z' ? c : c + 32; }
