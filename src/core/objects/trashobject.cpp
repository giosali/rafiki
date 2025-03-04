#include "trashobject.h"

#include <cstdlib>
#include <iterator>
#include <system_error>
#include <vector>

TrashObject::TrashObject(FeatureModel* model) : FeatureObject{model} {}

void TrashObject::Drag() {}

void TrashObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      FeatureObject::ProcessKeyPress(combination);
      break;
    case Qt::Key_Return:
      emit Hidden();
      EmptyTrash();
      FeatureObject::ProcessKeyPress(combination);
      break;
    default:
      break;
  }
}

void TrashObject::ProcessKeyRelease(const QKeyCombination& combination) {}

void TrashObject::EmptyTrash() const {
  auto ec = std::error_code{};
  if (!std::filesystem::exists(path_, ec)) {
    return;
  }

  auto subdirs =
    std::vector<std::filesystem::path>{path_ / "files", path_ / "info"};
  for (const auto& subdir : subdirs) {
    if (!std::filesystem::exists(subdir, ec)) {
      continue;
    }

    // Removes files from user's Trash through std::filesystem.
    for (const auto& entry : std::filesystem::directory_iterator{
           subdir, std::filesystem::directory_options::skip_permission_denied,
           ec}) {
      std::filesystem::remove_all(entry, ec);
    }

    // Checks for unsuccessful deletion attempts of files.
    if (size_t file_count = std::distance(
          std::filesystem::directory_iterator{
            subdir, std::filesystem::directory_options::skip_permission_denied,
            ec},
          {});
        file_count == 0) {
      continue;
    }

    // Removes files through `rm` command.
    // This is intended to handle bizarre issues with 'stubborn' symlinks. This
    // is a last resort.
    //
    // Wrapped in double quotes to handle restricted characters such as spaces.
    auto quoted_path = "\"" + subdir.string() + "/\"*";
    auto cmd = "rm -rf " + quoted_path;
    std::system(cmd.c_str());
  }
}
