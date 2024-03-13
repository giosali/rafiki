#include "autocompletemap.h"

std::set<QString> AutocompleteMap::Find(const QString& text) const {
  if (auto it = map_.find(text); it != map_.end()) {
    return it->second;
  }

  return std::set<QString>{};
}

void AutocompleteMap::Insert(const QString& text) {
  auto key = QString{};
  for (const auto ch : text) {
    key += ch;

    // If the set does not exist when calling operator[], it will
    // automatically be default-initialized.
    map_[key].insert(text);
  }
}
