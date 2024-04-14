#ifndef FINDER_H
#define FINDER_H

#include <filesystem>
#include <set>
#include <string>
#include <vector>

class Finder {
 public:
  Finder() = default;
  explicit Finder(const std::string& location, size_t max_count,
                  const std::vector<std::filesystem::path> home_dirs);

  std::vector<std::filesystem::path> Search(const std::string& input);

 private:
  std::vector<std::filesystem::path> Iterate(const std::filesystem::path& path,
                                             const std::string& input);
  std::vector<std::filesystem::path> Reiterate(
    const std::filesystem::path& path, const std::string& input,
    std::vector<std::filesystem::path>& entries);

  size_t count_;
  std::vector<std::filesystem::path> files_;
  std::set<std::string> home_dir_names_;
  std::vector<std::filesystem::path> home_dirs_;
  std::set<std::string> ignored_dirs_;
  std::string location_;
  size_t max_count_;
};

#endif  // FINDER_H
