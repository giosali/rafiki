#include "trash.h"

#include <QStandardPaths>
#include <Qt>
#include <cstdlib>
#include <iterator>
#include <system_error>
#include <vector>

#include "../core/definitions.h"
#include "../core/io.h"

Trash::Trash()
    : BaseResult{kId,       kIcon,        kTitle,   kTitlePlaceholder,
                 kAltTitle, kDescription, kCommand, kAppendSpaceToCommand} {
  auto home_location =
    QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  auto home = std::filesystem::path{home_location.toStdString()};
  path_ = home / ".local/share/Trash";
}

void Trash::Drag() {}

void Trash::ProcessKeyPress(const QKeyCombination& combination,
                            const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      if (auto command = FormatCommand(); input.GetCmd() != command) {
        emit NewSearchBoxTextRequested(command);
      }

      break;
    case Qt::Key_Return:
      emit Hidden();
      Empty();
      break;
  }
}

void Trash::ProcessKeyRelease(const QKeyCombination& combination,
                              const Input& input) {
  return;
}

const QString Trash::kAltTitle{};

const bool Trash::kAppendSpaceToCommand{false};

const QString Trash::kCommand{"empty trash"};

const QString Trash::kDescription{};

const QString Trash::kIcon{Io::GetImageFilePath(defs::ImageFile::kTrash)};

const QString Trash::kId{"9056b806-9219-4f6a-b180-bb087b892bae"};

const QString Trash::kTitle{"Empty trash"};

const QString Trash::kTitlePlaceholder{};

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
