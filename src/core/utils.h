#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <numeric>
#include <string>
#include <vector>

namespace utils {
static const std::string kOutputFile{"/tmp/rafiki_output.txt"};

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

inline QString Format(QString fmt, const QString& arg) {
  return fmt.replace(fmt.indexOf("{}"), 2, arg);
}

inline std::string Join(const std::vector<std::string>& vec,
                        const std::string& delimiter) {
  return vec.empty() ? ""
                     : std::accumulate(
                         vec.begin(), vec.end(), std::string{},
                         [&delimiter](const std::string& lhs,
                                      const std::string& rhs) -> std::string {
                           return lhs + (lhs.empty() ? "" : delimiter) + rhs;
                         });
}

inline char ToLower(char c) { return c < 'A' || c > 'Z' ? c : c + 32; }

inline std::vector<std::string> Split(const std::string& str, char ch = ' ') {
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
  // Prevents std::lenght_error from being thrown if the input is empty.
  if (input.empty()) {
    return {};
  }

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
