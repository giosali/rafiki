#include "application.h"

#include <QDesktopServices>
#include <QIcon>
#include <QPixmapCache>
#include <QUuid>
#include <cstdlib>
#include <filesystem>
#include <thread>

#include "../core/config.h"
#include "../ui/mainwindow.h"

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

  pixmap_key_ =
    QPixmapCache::insert(QIcon(icon).pixmap(Config::search_result_icon_size_));
}

QString Application::DragAndDrop() { return QString{}; };

void Application::ProcessKeyPress(const QKeyCombination& combination,
                                  QWidget* parent) {
  auto main_window = MainWindow::Get();
  if (main_window == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Return: {
      if (combination.keyboardModifiers() & Qt::AltModifier) {
        // TODO: inform user that the action did not work.
        if (description_.isEmpty()) {
          break;
        }

        main_window->Hide();
        auto path = std::filesystem::path{description_.toStdString()};
        auto parent = path.parent_path();
        auto dir = QUrl::fromLocalFile(QString::fromStdString(parent));
        QDesktopServices::openUrl(dir);
        break;
      }

      main_window->Hide();

      // This does not execute when running through VSCode.
      auto t =
        std::thread{[this]() { std::system(exec_.toStdString().c_str()); }};
      t.detach();
      break;
    }
    case Qt::Key_Alt:
      main_window->SetSearchResultDescription(kAltDescription);
      break;
  }
}

void Application::ProcessKeyRelease(const QKeyCombination& combination,
                                    QWidget* parent) {
  auto main_window = MainWindow::Get();
  if (main_window == nullptr) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Alt:
      main_window->SetSearchResultDescription(description_);
      break;
  }
}
