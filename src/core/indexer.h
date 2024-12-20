#pragma once

#include <QString>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "models/featuremodel.h"
#include "tsl/htrie_map.h"

class Indexer final {
 public:
  static Indexer& GetInstance();
  void Clear();
  void DeleteModel(uint64_t id);
  std::unordered_set<uint64_t> GetIds(const QString& input) const;
  FeatureModel* GetModel(uint64_t id) const;
  template <typename T>
  std::vector<T*> GetModels() const {
    auto models = std::vector<T*>{};
    for (const auto& pair : models_map_) {
      auto model = pair.second.get();
      if (auto check = dynamic_cast<T*>(model); check != nullptr) {
        models.push_back(check);
      }
    }

    return models;
  }
  void IndexModel(std::unique_ptr<FeatureModel> model);
  void Initialize();
  void UpdateTrie(uint64_t id,
                  const std::unordered_set<std::string>& old_tokens,
                  const std::unordered_set<std::string>& new_tokens);

 private:
  Indexer() = default;
  Indexer(const Indexer&) = delete;

  void IndexApplications();
  void IndexGenericModels();
  void IndexWebSearches();
  Indexer operator=(const Indexer&) = delete;

  std::unordered_set<uint64_t> commandless_ids_{};
  tsl::htrie_map<char, std::unordered_set<uint64_t>> input_models_trie_{};
  tsl::htrie_map<char, std::unordered_set<uint64_t>> inputless_models_trie_{};
  std::unordered_map<uint64_t, std::unique_ptr<FeatureModel>> models_map_{};
};
