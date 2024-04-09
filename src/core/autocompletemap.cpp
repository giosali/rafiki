#include "autocompletemap.h"

#include <vector>

std::set<QString> AutocompleteMap::Find(const Input& input) const {
  auto concat = std::set<QString>{};

  auto keys = input.IsCmdRedundant()
                ? std::vector<QString>{input.GetCmd()}
                : std::vector<QString>{input.GetCmd(), input.GetFull()};
  for (const auto& key : keys) {
    if (auto it = map_.find(key); it != map_.end()) {
      auto s = it->second;
      concat.insert(s.begin(), s.end());
    }
  }

  return concat;
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
