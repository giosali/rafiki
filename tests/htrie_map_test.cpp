#include "htrie_map_test.h"

#include <cstdint>
#include <unordered_set>

#include "tsl/htrie_map.h"

void TestHtrieMap::EqualPrefixRange_CommonPrefix() {
  auto trie = tsl::htrie_map<char, std::unordered_set<uint64_t>>{};

  auto pair = trie.insert("hello", std::unordered_set<uint64_t>{});
  pair.first->insert(1);

  pair = trie.insert("hey", std::unordered_set<uint64_t>{});
  pair.first->insert(2);

  auto ids = std::vector<uint64_t>{};
  auto range = trie.equal_prefix_range("he");
  for (auto it = range.first; it != range.second; ++it) {
    for (auto id : it.value()) {
      ids.push_back(id);
    }
  }

  auto expected = 2;
  auto actual = ids.size();
  QCOMPARE(actual, expected);
}

void TestHtrieMap::EqualPrefixRange_EraseAfterInsert() {
  auto trie = tsl::htrie_map<char, std::unordered_set<uint64_t>>{};

  auto pair = trie.insert("hello", std::unordered_set<uint64_t>{});
  pair.first->insert(1);

  pair = trie.insert("hey", std::unordered_set<uint64_t>{});
  pair.first->insert(2);

  trie.erase("hello");

  auto ids = std::vector<uint64_t>{};
  auto range = trie.equal_prefix_range("he");
  for (auto it = range.first; it != range.second; ++it) {
    for (auto id : it.value()) {
      ids.push_back(id);
    }
  }

  auto expected = 1;
  auto actual = ids.size();
  QCOMPARE(actual, expected);
}

void TestHtrieMap::EqualPrefixRange_EraseSharedKey() {
  auto trie = tsl::htrie_map<char, std::unordered_set<uint64_t>>{};

  auto pair = trie.insert("hello", std::unordered_set<uint64_t>{});
  pair.first->insert(1);

  pair = trie.insert("hello", std::unordered_set<uint64_t>{});
  pair.first->insert(2);

  trie.erase("hello");

  auto ids = std::vector<uint64_t>{};
  auto range = trie.equal_prefix_range("he");
  for (auto it = range.first; it != range.second; ++it) {
    for (auto id : it.value()) {
      ids.push_back(id);
    }
  }

  auto expected = 0;
  auto actual = ids.size();
  QCOMPARE(actual, expected);
}

void TestHtrieMap::Find_EraseSharedKeyAndKeepRemainder() {
  auto trie = tsl::htrie_map<char, std::unordered_set<uint64_t>>{};

  auto pair = trie.insert("hello", std::unordered_set<uint64_t>{});
  pair.first->insert(1);

  pair = trie.insert("hello", std::unordered_set<uint64_t>{});
  pair.first->insert(2);

  auto it = trie.find("hello");
  auto& value = it.value();
  value.erase(1);

  auto ids = std::vector<uint64_t>{};
  auto range = trie.equal_prefix_range("he");
  for (auto it = range.first; it != range.second; ++it) {
    for (auto id : it.value()) {
      ids.push_back(id);
    }
  }

  auto expected = 1;
  auto actual = ids.size();
  QCOMPARE(actual, expected);
}
