#pragma once

#include <QString>
#include <cstdint>
#include <string>

class Crypto {
 public:
  Crypto() = delete;

  static uint64_t Djb2(const QString& key);
  static uint64_t Djb2(const std::string& key);
};
