#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <string>
#include <vector>

class Utilities {
 public:
  Utilities() = delete;

  static std::vector<QString> Split(const QString& str, const QChar& delimiter,
                                    int maxsplit = -1);
  static std::vector<std::string> Split(const std::string& str, char delimiter,
                                        int maxsplit = -1);
  static char ToLower(char c);
};

#endif  // UTILITIES_H
