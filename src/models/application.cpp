#include "application.h"

#include <QDesktopServices>
#include <QIcon>
#include <QPixmapCache>
#include <QUrl>
#include <QUuid>
#include <cstdlib>
#include <thread>

#include "../core/crypto.h"

Application::Application(const std::filesystem::path& desktop_entry_path,
                         QSettings& desktop_entry_file, const QString& icon)
    : exec_{desktop_entry_file.value("Exec").toString()} {
  desktop_entry_file.beginGroup("Desktop Entry");
  auto id = Crypto::Djb2(desktop_entry_path);
  auto name = desktop_entry_file.value("Name").toString();
  auto description = QString::fromStdString(desktop_entry_path);

  SetIcon(icon);
  SetId(id);
  SetTitle(name);
  SetCommand(name);
  SetDescription(description);

  desktop_entry_file.endGroup();
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
    default:
      break;
  }
}

void Application::ProcessKeyRelease(const QKeyCombination& combination,
                                    const Input& input) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewDescriptionRequested(GetDescription());
      break;
    default:
      break;
  }
}
