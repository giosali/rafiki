#include "filesystementrybridge.h"

#include <algorithm>

#include "../models/filesystementrymodel.h"
#include "../objects/filesystementryobject.h"
#include "../utilities.h"

std::vector<std::shared_ptr<FeatureObject>> FileSystemEntryBridge::ProcessInput(
  const FeatureModel* feature_model, const QString& input) {
  auto model = static_cast<const FileSystemEntryModel*>(feature_model);
  auto command = model->GetCommand();

  auto objects = std::vector<std::shared_ptr<FeatureObject>>{};

  // Returns a basic informational result since the current input is equal to
  // the command.
  if (input == command) {
    objects.push_back(std::make_shared<FileSystemEntryObject>(model));
    return objects;
  }

  // Converts found std::filesystem::path objects into FileSystemEntryObject
  // objects.
  auto paths = finder_.Search(input.sliced(command.length()).toStdString());
  auto paths_size = paths.size();
  objects.reserve(paths_size);
  for (size_t i = 0; i < paths_size; ++i) {
    objects.push_back(std::make_shared<FileSystemEntryObject>(model, paths[i]));
  }

  return objects;
}

FileSystemEntryBridge::Finder::Finder(
  const std::string& location, size_t max_count,
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

std::vector<std::filesystem::path> FileSystemEntryBridge::Finder::Search(
  const std::string& input) {
  count_ = 0;
  return Iterate(location_, input);
}

std::vector<std::filesystem::path> FileSystemEntryBridge::Finder::Iterate(
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
                   Utilities::ToLower);
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

std::vector<std::filesystem::path> FileSystemEntryBridge::Finder::Reiterate(
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
                   Utilities::ToLower);
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

const std::unordered_map<std::string, std::pair<QString, QString>>
  FileSystemEntryBridge::kMimeTypePairs{
    // <extension, [preferred MIME type, fallback MIME type]>
    // Audio
    {".flac", {"audio-x-flac", "audio-x-generic"}},
    {".mp3", {"audio-x-mpeg", "audio-x-generic"}},
    {".opus", {"audio-x-generic", "audio-x-generic"}},
    {".wav", {"audio-x-wav", "audio-x-generic"}},
    // Archive
    {".gz", {"application-x-gzip", "application-x-gzip"}},
    {".zip", {"application-x-zip", "application-x-zip"}},
    // Misc
    {"", {"folder", "folder"}},
    // Image
    {".jpg", {"image-x-generic", "image-x-generic"}},
    {".jpeg", {"image-x-generic", "image-x-generic"}},
    {".png", {"image-x-generic", "image-x-generic"}},
    {".svg", {"image-svg+xml", "image-x-generic"}},
    // Text
    {".c", {"text-x-c", "text-x-generic"}},
    {".cpp", {"text-x-cpp", "text-x-generic"}},
    {".css", {"text-css", "text-x-generic"}},
    {".hpp", {"text-x-c++hdr", "text-x-generic"}},
    {".md", {"text-markdown", "text-x-generic"}},
    {".py", {"text-x-python", "text-x-generic"}},
    {".rs", {"text-rust", "text-x-generic"}},
    {".rtf", {"text-richtext", "text-x-generic"}},
    {".sass", {"text-x-sass", "text-x-generic"}},
    {".scss", {"text-x-sass", "text-x-generic"}},
    {".ts", {"text-x-typescript", "text-x-generic"}},
    {".txt", {"text-x-generic", "text-x-generic"}},
    // Video
    {".mp4", {"video-x-generic", "video-x-generic"}},
    {".webm", {"video-x-generic", "video-x-generic"}},
  };
