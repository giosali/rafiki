#include "application.h"

#include <QDesktopServices>
#include <QIcon>
#include <QPixmapCache>
#include <QUrl>
#include <QUuid>
#include <cstdlib>
#include <filesystem>
#include <thread>

#include "../core/config.h"
#include "../core/crypto.h"

Application::Application(const QString& desktop_entry_path, const QString& name,
                         const QString& icon, uintmax_t icon_size,
                         const QString& exec)
    : Result{Crypto::Djb2(desktop_entry_path)}, exec_{exec} {
  SetIcon(icon);
  SetTitle(name);
  SetCommand(name);
  SetDescription(desktop_entry_path);
  SetPixmapKey(icon, icon_size);
}

void Application::Drag() {};

void Application::ProcessKeyPress(const QKeyCombination& combination,
                                  const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Return: {
      if (combination.keyboardModifiers() & Qt::AltModifier) {
        // TODO: inform user that the action did not work.
        auto description = Description();
        if (description.isEmpty()) {
          break;
        }

        emit Hidden();
        auto path = std::filesystem::path{description.toStdString()};
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
      emit NewDescriptionRequested("Reveal in folder");
      break;
  }
}

void Application::ProcessKeyRelease(const QKeyCombination& combination,
                                    const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewDescriptionRequested(Description());
      break;
  }
}
