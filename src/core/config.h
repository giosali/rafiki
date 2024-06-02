#ifndef CONFIG_H
#define CONFIG_H

class Config {
 public:
  Config() = delete;

  static int search_result_icon_size_;
  static int search_result_list_max_count_;
  static int search_result_title_max_height_;
};

#endif  // CONFIG_H
