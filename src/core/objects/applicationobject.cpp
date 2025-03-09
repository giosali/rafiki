#include "applicationobject.h"

#include <QDesktopServices>
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QUrl>
#include <cstdlib>
#include <thread>

#include "../models/applicationmodel.h"

ApplicationObject::ApplicationObject(FeatureModel* model)
    : FeatureObject{model} {}

void ApplicationObject::Drag() {}

void ApplicationObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      FeatureObject::ProcessKeyPress(combination);
      break;
    case Qt::Key_Return: {
      emit Hidden();

      if (combination.keyboardModifiers() & Qt::AltModifier) {
        auto parent_path = QFileInfo{GetDescription()}.path();
        auto directory = QUrl::fromLocalFile(parent_path);
        QDesktopServices::openUrl(directory);
      } else if (!QStandardPaths::findExecutable("gtk-launch").isEmpty()) {
        // `gtk-launch` is much quicker than using QProcess.

        // The command in "exec" does not execute when running through VSCode.
        // The command needs to be passed by value rather than reference
        // otherwise it will contain garbage characters/gibberish.
        auto command = QString{"gtk-launch %1"}
                         .arg(QFileInfo{GetDescription()}.fileName())
                         .toStdString();
        auto t = std::jthread{[command] { std::system(command.c_str()); }};
        t.detach();
      } else if (auto model = dynamic_cast<const ApplicationModel*>(GetModel());
                 model != nullptr) {
        // Fallback to QProcess.
        auto exec =
          model->GetExec().remove(QRegularExpression{"%[fFuUdDnNickvm]"});
        auto exec_parts = exec.split(' ', Qt::SkipEmptyParts);
        if (exec_parts.isEmpty()) {
          qWarning() << "exec_parts is empty";
          break;
        }

        // Some applications won't launch normally when the QT_QPA_PLATFORM is
        // set to `xcb`. This application's QT_QPA_PLATFORM value is set to
        // `xcb` by default. Therefore, we need to look at the user's session
        // and use that to determine whether applications should be launched
        // using `x11` or `wayland`.
        auto env = QProcessEnvironment::systemEnvironment();
        auto xdg_session_type = env.value("XDG_SESSION_TYPE", "wayland");
        auto platform = xdg_session_type == "x11" ? "x11" : "wayland";
        env.insert("QT_QPA_PLATFORM", platform);

        auto program = exec_parts.takeFirst();
        auto process = new QProcess{};
        process->setProgram(program);
        process->setProcessEnvironment(env);
        process->setArguments(exec_parts);

        QObject::connect(process, &QProcess::finished, process,
                         &QProcess::deleteLater);
        QObject::connect(
          process, &QProcess::errorOccurred, [=](QProcess::ProcessError error) {
            qWarning() << "Error launching:" << program << "Error:" << error;
            process->deleteLater();
          });

        process->startDetached();
      } else {
        break;
      }

      FeatureObject::ProcessKeyPress(combination);
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
