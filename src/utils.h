#ifndef UTILS_H
#define UTILS_H

#include <QString>

namespace utils {

inline QString Format(QString fmt, const QString& arg) {
  return fmt.replace(fmt.indexOf("{}"), 2, arg);
}

}  // namespace utils

#endif  // UTILS_H
