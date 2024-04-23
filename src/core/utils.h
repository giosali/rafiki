#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <string>

namespace utils {

inline QString Capitalize(QString str) {
  return str.replace(0, 1, str.at(0).toUpper());
}

inline QString Format(QString fmt, const QString& arg) {
  return fmt.replace(fmt.indexOf("{}"), 2, arg);
}

inline char ToLower(char c) { return c < 'A' || c > 'Z' ? c : c + 32; }

inline std::string Strip(const std::string& input, char token = '\0') {
  auto start_it = input.begin();
  for (char ch = *start_it; std::isspace(ch) || ch == token;
       ++start_it, ch = *start_it) {
  }

  auto end_it = input.rbegin();
  for (char ch = *end_it; std::isspace(ch) || ch == token;
       ++end_it, ch = *end_it) {
  }

  return std::string{start_it, end_it.base()};
}

}  // namespace utils

#endif  // UTILS_H
