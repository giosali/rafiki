#include "finder.h"

#include <algorithm>

#include "../core/utils.h"

Finder::Finder(const std::string& location, size_t max_count,
               const std::vector<std::filesystem::path> home_dirs)
    : home_dirs_{home_dirs}, location_{location}, max_count_{max_count} {
  // Extracts the name of the directory only for future comparison when
  // iterating over entries in the user's home directory.
  for (const auto& dir : home_dirs) {
    home_dir_names_.insert(dir.filename());
  }

  // Sets directories that should be ignored while searching.
  ignored_dirs_.emplace("node_modules");   // npm
  ignored_dirs_.emplace("site-packages");  // Python
}

std::vector<std::filesystem::path> Finder::Search(const std::string& input) {
  count_ = 0;
  return Iterate(location_, input);
}

std::vector<std::filesystem::path> Finder::Iterate(
  const std::filesystem::path& path, const std::string& input) {
  auto entries = std::vector<std::filesystem::path>{};
  auto other_dirs = std::vector<std::filesystem::path>{};

  for (const auto& entry : std::filesystem::directory_iterator{
         path, std::filesystem::directory_options::skip_permission_denied}) {
    auto filename = entry.path().filename().string();
    if (filename[0] == '.') {
      continue;
    }

    if (entry.is_directory() &&
        home_dir_names_.find(filename) == home_dir_names_.end()) {
      other_dirs.push_back(entry);
    }

    // Skips the entry if the input is longer than the filename.
    if (input.length() > filename.length()) {
      continue;
    }

    // Skips entry if its name doesn't contain the input.
    std::transform(filename.begin(), filename.end(), filename.begin(),
                   utils::ToLower);
    if (filename.find(input) == std::string::npos) {
      continue;
    }

    entries.push_back(entry);
    if (++count_ == max_count_) {
      return entries;
    }
  }

  for (const auto& dir : home_dirs_) {
    Reiterate(dir, input, entries);
    if (count_ == max_count_) {
      return entries;
    }
  }

  for (const auto& dir : other_dirs) {
    Reiterate(dir, input, entries);
    if (count_ == max_count_) {
      return entries;
    }
  }

  return entries;
}

std::vector<std::filesystem::path> Finder::Reiterate(
  const std::filesystem::path& path, const std::string& input,
  std::vector<std::filesystem::path>& entries) {
  auto dirs = std::vector<std::filesystem::path>{};

  for (const auto& entry : std::filesystem::directory_iterator{
         path, std::filesystem::directory_options::skip_permission_denied}) {
    auto filename = entry.path().filename().string();
    if (filename[0] == '.') {
      continue;
    }

    if (entry.is_directory()) {
      if (ignored_dirs_.find(filename) != ignored_dirs_.end()) {
        continue;
      }

      dirs.push_back(entry);
    }

    // Skips the entry if the input is longer than the filename.
    if (input.length() > filename.length()) {
      continue;
    }

    // Skips entry if its name doesn't contain the input.
    std::transform(filename.begin(), filename.end(), filename.begin(),
                   utils::ToLower);
    if (filename.find(input) == std::string::npos) {
      continue;
    }

    entries.push_back(entry);
    if (++count_ == max_count_) {
      return entries;
    }
  }

  for (const auto& dir : dirs) {
    Reiterate(dir, input, entries);
    if (count_ == max_count_) {
      return entries;
    }
  }

  return entries;
}
