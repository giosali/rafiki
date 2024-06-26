#include "crypto.h"

uint64_t Crypto::Djb2(const QString& key) {
  uint64_t hash = 5381;
  for (const auto& c : key) {
    hash = hash * 33 + c.unicode();
  }

  return hash;
}

uint64_t Crypto::Djb2(const std::string& key) {
  uint64_t hash = 5381;
  for (size_t i = 0; i < key.size(); ++i) {
    hash = hash * 33 + key[i];
  }

  return hash;
}
