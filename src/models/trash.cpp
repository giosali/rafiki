#include "trash.h"

#include <QStandardPaths>
#include <Qt>
#include <cstdlib>
#include <filesystem>
#include <iterator>
#include <system_error>
#include <vector>

#include "../ui/searchresultlist.h"

Trash::Trash()
    : BaseResult{kId,       kIcon,        kTitle,   kTitlePlaceholder,
                 kAltTitle, kDescription, kCommand, kAppendSpaceToCommand} {
  auto home_location =
    QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  auto home = std::filesystem::path{home_location.toStdString()};
  auto trash = home / ".local/share/Trash";
  path_ = trash.c_str();
}

QString Trash::DragAndDrop() { return QString{}; }

void Trash::ProcessKeyPress(const QKeyCombination& combination,
                            QWidget* parent) {
  auto search_result_list = dynamic_cast<SearchResultList*>(parent);
  if (search_result_list == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Tab:
      if (auto command = FormatCommand();
          search_result_list->GetCmd() != command) {
        emit search_result_list->TextReceived(command);
      }

      break;

    case Qt::Key_Return:
      search_result_list->HideParent();
      Empty();
      break;
  }
}

void Trash::ProcessKeyRelease(const QKeyCombination& combination,
                              QWidget* parent) {
  return;
}

void Trash::Empty() const {
  auto trash = std::filesystem::path{path_};
  if (!std::filesystem::exists(trash)) {
    return;
  }

  auto subdirs =
    std::vector<std::filesystem::path>{trash / "files", trash / "info"};
  for (const auto& subdir : subdirs) {
    if (!std::filesystem::exists(subdir)) {
      continue;
    }

    // Removes files from user's Trash through std::filesystem.
    for (const auto& entry : std::filesystem::recursive_directory_iterator{
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
