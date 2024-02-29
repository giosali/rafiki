#include "autocompletemap.h"

std::set<std::string> AutocompleteMap::Find(const std::string& text) const {
  if (auto search = map_.find(text); search != map_.end()) {
    return search->second;
  }

  return std::set<std::string>();
}

void AutocompleteMap::Insert(const std::string& text) {
  auto key = std::string();
  for (const auto ch : text) {
    key += ch;

    // If the set does not exist when calling operator[], it will
    // automatically be default-initialized.
    map_[key].insert(text);
  }
}
