#include "application.h"

#include <QDesktopServices>
#include <QIcon>
#include <QPixmapCache>
#include <QUuid>
#include <cstdlib>
#include <filesystem>
#include <thread>

#include "../ui/searchresultlist.h"

Application::Application(const QString& name, const QString& icon,
                         uintmax_t icon_size, const QString& description,
                         const QString& exec)
    : BaseResult{QUuid::createUuid().toString(),
                 icon,
                 name,
                 QString{},
                 QString{},
                 description,
                 name,
                 false},
      exec_{exec} {
  if (icon_size < 1000000) {  // 1 MB
    return;
  }

  pixmap_key_ = QPixmapCache::insert(QIcon(icon).pixmap(44));
}

QString Application::DragAndDrop() { return QString{}; };

void Application::ProcessKeyPress(const QKeyCombination& combination,
                                  QWidget* parent) {
  auto search_result_list = dynamic_cast<SearchResultList*>(parent);
  if (search_result_list == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Return: {
      if (combination.keyboardModifiers() & Qt::AltModifier) {
        // TODO: inform user that the action did not work.
        if (description_.isEmpty()) {
          break;
        }

        search_result_list->HideParent();
        auto path = std::filesystem::path{description_.toStdString()};
        auto parent = path.parent_path();
        auto dir = QUrl::fromLocalFile(QString::fromStdString(parent));
        QDesktopServices::openUrl(dir);
        break;
      }

      search_result_list->HideParent();
      auto t = std::thread{
        [](const QString& exec) { std::system(exec.toStdString().c_str()); },
        exec_};
      t.detach();
      break;
    }
    case Qt::Key_Alt:
      search_result_list->CurrentSearchResult()->SetDescription(
        kAltDescription);
      break;
  }
}

void Application::ProcessKeyRelease(const QKeyCombination& combination,
                                    QWidget* parent) {
  auto search_result_list = dynamic_cast<SearchResultList*>(parent);
  if (search_result_list == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Alt:
      search_result_list->CurrentSearchResult()->SetDescription(description_);
      break;
  }
}
