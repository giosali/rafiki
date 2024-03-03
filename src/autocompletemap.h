#ifndef AUTOCOMPLETEMAP_H
#define AUTOCOMPLETEMAP_H

#include <QHash>
#include <QString>
#include <set>
#include <string>
#include <unordered_map>

class AutocompleteMap {
 public:
  explicit AutocompleteMap() = default;

  std::set<QString> Find(const QString& text) const;
  void Insert(const QString& text);

 private:
  std::unordered_map<QString, std::set<QString>> map_;
};

#endif  // AUTOCOMPLETEMAP_H
