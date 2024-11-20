#include "filesystementryobject.h"

#include <QDesktopServices>
#include <QUrl>
#include <Qt>

#include "../crypto.h"

FileSystemEntryObject::FileSystemEntryObject(const FileSystemEntryModel* model)
    : FeatureObject{model}, model_{model} {}

FileSystemEntryObject::FileSystemEntryObject(const FileSystemEntryModel* model,
                                             const std::filesystem::path& path)
    : FeatureObject{model, {}}, model_{model}, path_{path} {
  SetDescription(QString::fromUtf8(path.string()));
  SetId(Crypto::Djb2(path));
  SetTitle(QString::fromUtf8(path.filename().string()));
}

void FileSystemEntryObject::Drag() {
  // Ensures only objects with paths are processed.
  if (!path_.empty()) {
    emit Dragged(GetDescription());
  }
}

void FileSystemEntryObject::ProcessKeyPress(
  const QKeyCombination& combination) {
  // Ensures only objects with paths are processed.
  if (path_.empty()) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Return: {
      // Opens either the containing directory of the entry or the entry itself.
      auto url =
        QUrl::fromLocalFile(combination.keyboardModifiers() & Qt::AltModifier
                              ? QString::fromStdString(std::filesystem::path{
                                  GetDescription().toStdString()}
                                                         .parent_path()
                                                         .string())
                              : GetDescription());
      emit Hidden();
      QDesktopServices::openUrl(url);
      break;
    }
    case Qt::Key_Alt:
      emit NewDescriptionRequested(GetAltDescription());
      break;
    default:
      break;
  }
}

void FileSystemEntryObject::ProcessKeyRelease(
  const QKeyCombination& combination) {
  // Ensures only objects with paths are processed.
  if (path_.empty()) {
    return;
  }

  switch (combination.key()) {
    case Qt::Key_Alt:
      emit NewDescriptionRequested(GetDescription());
      break;
    default:
      break;
  }
}
