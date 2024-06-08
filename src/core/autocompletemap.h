#ifndef AUTOCOMPLETEMAP_H
#define AUTOCOMPLETEMAP_H

#include <QHash>
#include <QString>
#include <set>
#include <string>
#include <unordered_map>

#include "input.h"

class AutocompleteMap {
 public:
  AutocompleteMap() = default;

  std::set<QString> Find(const Input& input) const;
  void Insert(const QString& text);

 private:
  std::unordered_map<QString, std::set<QString>> map_;
};

#endif  // AUTOCOMPLETEMAP_H
