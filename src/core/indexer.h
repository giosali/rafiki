#ifndef INDEXER_H
#define INDEXER_H

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
  void Initialize();
  void ToggleModel(uint64_t id) const;

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
