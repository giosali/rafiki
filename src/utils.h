#ifndef UTILS_H
#define UTILS_H

#include <QString>

namespace utils {

inline QString Capitalize(QString str) {
  return str.replace(0, 1, str.at(0).toUpper());
}

inline QString Format(QString fmt, const QString& arg) {
  return fmt.replace(fmt.indexOf("{}"), 2, arg);
}

}  // namespace utils

#endif  // UTILS_H
