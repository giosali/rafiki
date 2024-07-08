#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

class Config {
 public:
  static const uint64_t kApplicationAuthorId;
  static const uint64_t kUserAuthorId;

  Config() = delete;

  static int search_result_icon_size_;
  static int search_result_list_max_count_;
  static int search_result_title_max_height_;
};

#endif  // CONFIG_H
