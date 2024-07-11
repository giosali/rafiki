#include "autocompleter.h"

#include <vector>

std::set<QString> Autocompleter::Find(const Input& input) const {
  auto concat = std::set<QString>{};

  auto keys = input.IsCommandRedundant()
                ? std::vector<QString>{input.Command()}
                : std::vector<QString>{input.Command(), input.ToString()};
  for (const auto& key : keys) {
    if (auto it = map_.find(key.toLower()); it != map_.end()) {
      auto s = it->second;
      concat.insert(s.begin(), s.end());
    }
  }

  return concat;
}

void Autocompleter::Insert(const QString& text) {
  auto key = QString{};
  auto acronym_key = QString{};

  for (size_t i = 0; i < text.length(); ++i) {
    auto ch = text[i];
    if (ch.isUpper()) {
      ch = ch.toLower();

      // Ultimately allows for keys such as "ProtonVPN" to be  accessed, for
      // example, through of the following consecutive letters: "pvpn"
      acronym_key += ch;
    }

    key += ch;

    // If the set does not exist when calling operator[], it will
    // automatically be default-initialized.
    // The key for the internal must only consist of lowercase characters; the
    // value can contain mixed-case.
    map_[key].insert(text);
  }

  // Allows for the actual "consecutive" part to be implemented (concerning the
  // acronym key). For example, "pvpn" from "ProtonVPN" breaks down (and is
  // stored) into the following:
  // -> "pvpn"
  // -> "vpn"
  // -> "pn"
  // -> "n"
  for (size_t i = 0, l = acronym_key.length(); i < l; ++i, key.clear()) {
    auto slice = acronym_key.sliced(i);

    auto key = QString{};
    for (const auto& ch : slice) {
      key += ch;
      map_[key].insert(text);
    }
  }
}
