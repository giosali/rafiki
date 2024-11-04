#include "trash.h"

#include <QStandardPaths>
#include <Qt>
#include <iterator>
#include <system_error>
#include <vector>

#include "../core/paths.h"

Trash::Trash()
    : path_{std::filesystem::path{
              QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                .toStdString()} /
            ".local/share/Trash"} {
  SetCommand("empty trash");
  SetIcon(Paths::Path(Paths::Image::kTrash));
  SetId(18);
  SetTitle("Empty trash");
}

void Trash::Drag() {}

void Trash::ProcessKeyPress(const QKeyCombination& combination,
                            const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      if (auto command = FormatCommand(); input.Command() != command) {
        emit NewSearchBoxTextRequested(command);
      }

      break;
    case Qt::Key_Return:
      emit Hidden();
      Empty();
      break;
    default:
      break;
  }
}

void Trash::ProcessKeyRelease(const QKeyCombination& combination,
                              const Input& input) {}

void Trash::Empty() const {
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
