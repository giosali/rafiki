#ifndef INDEXER_H
#define INDEXER_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "models/featuremodel.h"
#include "tsl/htrie_map.h"

class Indexer final {
 public:
  static Indexer& GetInstance();
  std::unordered_set<uint64_t> GetIds(const std::string& input) const;
  FeatureModel* GetModel(uint64_t id) const;
  void Initialize();

 private:
  Indexer() = default;
  Indexer(const Indexer&) = delete;

  void IndexApplications();
  void IndexModel(std::unique_ptr<FeatureModel> model);
  void IndexGenericModels();
  void IndexWebSearches();
  Indexer operator=(const Indexer&) = delete;

  std::unordered_set<uint64_t> commandless_ids_{};
  std::unordered_map<uint64_t, std::unique_ptr<FeatureModel>> models_map_{};
  tsl::htrie_map<char, std::unordered_set<uint64_t>> models_trie_{};
};

#endif  // INDEXER_H
