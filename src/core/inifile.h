#ifndef INIFILE_H
#define INIFILE_H

#include <any>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>

class IniFile {
 public:
  explicit IniFile(const std::string& file_path);

  void BeginSection(const std::string& section);
  void EndSection();
  /// @brief Finds element with specific key.
  /// @tparam T Supported types: `bool`, `int`, and `std::string`
  /// @param key
  /// @param fallback
  /// @return
  template <typename T>
  std::any GetValue(const std::string& key,
                    const std::any& fallback = std::any{}) const {
    auto it = properties_.find(section_.empty() ? key : section_ + "/" + key);
    if (std::is_same<T, bool>::value) {
      // Returns false if the key wasn't found.
      if (it == properties_.end()) {
        return false;
      }

      auto v = it->second;
      if ((v == "1" || v == "yes" || v == "true" || v == "on")) {
        return true;
      }

      return fallback.has_value() ? fallback : false;
    } else if (std::is_same<T, int>::value) {
      // Returns 0 if the key wasn't found.
      if (it == properties_.end()) {
        return 0;
      }

      try {
        return std::stoi(it->second);
      } catch (const std::invalid_argument&) {
        return 0;
      } catch (const std::out_of_range&) {
        return 0;
      }
    } else {
      // If the key exists, returns the value.
      // Otherwise, if the fallback has a value, returns the fallback.
      // Otherwise, returns an empty string.
      return it != properties_.end()
               ? it->second
               : (fallback.has_value() ? fallback : std::string{});
    }
  }

 private:
  void Parse(const std::string& file_path);

  std::map<std::string, std::string> properties_;
  std::string section_;
};

#endif  // INIFILE_H
