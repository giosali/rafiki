#include "trash.h"

#include <Qt>
#include <cstdlib>
#include <filesystem>
#include <vector>

#include "../ui/searchresultlist.h"

Trash::Trash()
    : BaseResult{kId,       kIcon,        kTitle,   kTitlePlaceholder,
                 kAltTitle, kDescription, kCommand, kAppendSpaceToCommand} {
  if (auto xdg_data_home = std::getenv("XDG_DATA_HOME");
      xdg_data_home != nullptr) {
    auto home_loc = std::string{xdg_data_home};
    trash_loc_ =
      home_loc.empty() ? "~/.local/share/Trash" : home_loc + "/Trash";
  }
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
  auto trash_path = std::filesystem::path{trash_loc_};
  auto paths = std::vector<std::filesystem::path>{trash_path / "files",
                                                  trash_path / "info"};
  for (const auto& path : paths) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      std::filesystem::remove_all(entry.path());
    }
  }
}
