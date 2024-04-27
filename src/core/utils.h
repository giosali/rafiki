#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace utils {
static const std::string kOutputFile{"/tmp/rafiki_output.txt"};

inline QString Capitalize(QString str) {
  return str.replace(0, 1, str.at(0).toUpper());
}

inline bool CompareStrings(const std::string& lhs_str,
                           const std::string& rhs_str, bool reverse = false) {
  auto r = std::lexicographical_compare(
    lhs_str.begin(), lhs_str.end(), rhs_str.begin(), rhs_str.end(),
    [](char lhs_ch, char rhs_ch) {
      auto proj = [](char ch) {
        return std::tuple(std::tolower(ch), std::isupper(ch));
      };
      return proj(lhs_ch) < proj(rhs_ch);
    });
  return reverse ? !r : r;
}

inline std::string Execute(const std::string& cmd) {
  auto status = std::system((cmd + " >" + kOutputFile).c_str());
  if (status != 0) {
    return {};
  }

  auto stream = std::ostringstream{};
  stream << std::ifstream{kOutputFile}.rdbuf();
  return stream.str();
}

inline QString Format(QString fmt, const QString& arg) {
  return fmt.replace(fmt.indexOf("{}"), 2, arg);
}

inline char ToLower(char c) { return c < 'A' || c > 'Z' ? c : c + 32; }

inline std::vector<std::string> Split(const std::string& str, char ch) {
  auto v = std::vector<std::string>{};
  for (size_t i = str.find(ch), last_i = 0;;
       last_i = ++i, i = str.find(ch, i)) {
    v.push_back(str.substr(last_i, i - last_i));
    if (i == std::string::npos) {
      break;
    }
  }

  return v;
}

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
