#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

class Utilities {
 public:
  Utilities() = delete;

  static std::vector<std::string> Split(const std::string& str,
                                        const std::string& delimiter,
                                        int maxsplit = -1);
  static char ToLower(char c);
};

#endif  // UTILITIES_H
