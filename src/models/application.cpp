#include "application.h"

#include <QDesktopServices>
#include <QIcon>
#include <QPixmapCache>
#include <QUuid>
#include <cstdlib>
#include <filesystem>
#include <thread>

#include "../core/config.h"

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
  if (icon_size >= 1000000) {  // 1 MB
    pixmap_key_ = QPixmapCache::insert(
      QIcon(icon).pixmap(Config::search_result_icon_size_));
  }
}

void Application::Drag() {};

void Application::ProcessKeyPress(const QKeyCombination& combination,
                                  const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Return: {
      if (combination.keyboardModifiers() & Qt::AltModifier) {
        // TODO: inform user that the action did not work.
        if (description_.isEmpty()) {
          break;
        }

        emit Hidden();
        auto path = std::filesystem::path{description_.toStdString()};
        auto parent = path.parent_path();
        auto dir = QUrl::fromLocalFile(QString::fromStdString(parent));
        QDesktopServices::openUrl(dir);
        break;
      }

      emit Hidden();

      // This does not execute when running through VSCode.
      auto t =
        std::thread{[this]() { std::system(exec_.toStdString().c_str()); }};
      t.detach();
      break;
    }
    case Qt::Key_Alt:
      emit NewDescriptionRequested(kAltDescription);
      break;
  }
}

void Application::ProcessKeyRelease(const QKeyCombination& combination,
                                    const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewDescriptionRequested(description_);
      break;
  }
}

const QString Application::kAltDescription{"Reveal in folder"};
