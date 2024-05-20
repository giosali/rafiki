#include "trash.h"

#include <QStandardPaths>
#include <Qt>
#include <cstdlib>
#include <iterator>
#include <system_error>
#include <vector>

#include "../ui/mainwindow.h"
#include "../ui/searchresultlist.h"

Trash::Trash()
    : BaseResult{kId,       kIcon,        kTitle,   kTitlePlaceholder,
                 kAltTitle, kDescription, kCommand, kAppendSpaceToCommand} {
  auto home_location =
    QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  auto home = std::filesystem::path{home_location.toStdString()};
  path_ = home / ".local/share/Trash";
}

QString Trash::DragAndDrop() { return QString{}; }

void Trash::ProcessKeyPress(const QKeyCombination& combination,
                            QWidget* parent) {
  auto main_window = MainWindow::Get();
  if (main_window == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Tab:
      if (auto command = FormatCommand();
          main_window->GetSearchBoxText().GetCmd() != command) {
        main_window->SetSearchBoxText(command);
      }

      break;

    case Qt::Key_Return:
      main_window->Hide();
      Empty();
      break;
  }
}

void Trash::ProcessKeyRelease(const QKeyCombination& combination,
                              QWidget* parent) {
  return;
}

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
