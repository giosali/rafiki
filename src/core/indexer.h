#ifndef INDEXER_H
#define INDEXER_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "../models/result.h"
#include "tsl/htrie_map.h"

class Indexer final {
 public:
  static Indexer& GetInstance();
  std::unordered_map<uint64_t, std::shared_ptr<Result>> GetResultsMap() const;
  tsl::htrie_map<char, std::unordered_set<uint64_t>> GetResultsTrie() const;
  void Initialize();

 private:
  Indexer() = default;
  Indexer(const Indexer&) = delete;

  void IndexApplications();
  void IndexWebSearches();
  Indexer operator=(const Indexer&) = delete;

  std::unordered_map<uint64_t, std::shared_ptr<Result>> results_map_{};
  tsl::htrie_map<char, std::unordered_set<uint64_t>> results_trie_{};
};

#endif  // INDEXER_H
