#ifndef AUTOCOMPLETEMAP_H
#define AUTOCOMPLETEMAP_H

#include <set>
#include <string>
#include <unordered_map>

class AutocompleteMap {
 public:
  explicit AutocompleteMap() = default;

  std::set<std::string> Find(const std::string& text) const;
  void Insert(const std::string& text);

 private:
  std::unordered_map<std::string, std::set<std::string>> map_;
};

#endif  // AUTOCOMPLETEMAP_H
