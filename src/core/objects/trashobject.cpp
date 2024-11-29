#include "trashobject.h"

#include <cstdlib>
#include <iterator>
#include <system_error>
#include <vector>

#include "../models/trashmodel.h"

TrashObject::TrashObject(const FeatureModel* model) : FeatureObject{model} {}

void TrashObject::Drag() {}

void TrashObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      if (auto model = dynamic_cast<const TrashModel*>(GetModel());
          model != nullptr) {
        emit NewSearchBoxTextRequested(model->GetCommand());
      }

      break;
    case Qt::Key_Return:
      emit Hidden();
      EmptyTrash();
      break;
    default:
      break;
  }
}

void TrashObject::ProcessKeyRelease(const QKeyCombination& combination) {}

void TrashObject::EmptyTrash() const {
  if (!std::filesystem::exists(path_)) {
    return;
  }

  auto subdirs =
    std::vector<std::filesystem::path>{path_ / "files", path_ / "info"};
  for (const auto& subdir : subdirs) {
    if (!std::filesystem::exists(subdir)) {
      continue;
    }

    // Removes files from user's Trash through std::filesystem.
    for (const auto& entry : std::filesystem::directory_iterator{
           subdir,
           std::filesystem::directory_options::skip_permission_denied}) {
      auto ec = std::error_code{};
      std::filesystem::remove_all(entry, ec);
    }

    // Checks for unsuccessful deletion attempts of files.
    if (size_t file_count = std::distance(
          std::filesystem::directory_iterator{
            subdir, std::filesystem::directory_options::skip_permission_denied},
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
