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
                         const QString& icon, const QString& exec)
    : exec_{exec} {
  SetIcon(icon);
  SetId(Config::kApplicationAuthorId, Crypto::Djb2(desktop_entry_path));
  SetTitle(name);
  SetCommand(name);
  SetDescription(desktop_entry_path);
}

void Application::Drag() {};

void Application::ProcessKeyPress(const QKeyCombination& combination,
                                  const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Return: {
      if (combination.keyboardModifiers() & Qt::AltModifier) {
        // TODO: inform user that the action did not work.
        auto description = GetDescription();
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
      emit NewDescriptionRequested(GetDescription());
      break;
  }
}
