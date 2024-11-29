#pragma once

#include <QStandardPaths>
#include <filesystem>

#include "featureobject.h"

class TrashObject final : public FeatureObject {
  Q_OBJECT

 public:
  explicit TrashObject(const FeatureModel* model);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  void EmptyTrash() const;

  std::filesystem::path path_{
    std::filesystem::path{
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
        .toStdString()} /
    ".local/share/Trash"};
};
