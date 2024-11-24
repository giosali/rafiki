#include "applicationobject.h"

#include <QDesktopServices>
#include <QUrl>
#include <Qt>
#include <cstdlib>
#include <filesystem>
#include <thread>

ApplicationObject::ApplicationObject(const ApplicationModel* model)
    : FeatureObject{model}, model_{model} {}

void ApplicationObject::Drag() {}

void ApplicationObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      FeatureObject::ProcessKeyPress(combination);
      break;
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
      auto exec = model_->GetExec().toStdString();
      auto t = std::jthread{[&exec]() { std::system(exec.c_str()); }};
      t.detach();
      break;
    }
    case Qt::Key_Alt:
      emit NewDescriptionRequested(GetAltDescription());
      break;
    default:
      break;
  }
}

void ApplicationObject::ProcessKeyRelease(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewDescriptionRequested(GetDescription());
      break;
    default:
      break;
  }
}
