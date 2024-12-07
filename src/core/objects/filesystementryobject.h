#pragma once

#include <QIcon>
#include <filesystem>

#include "../models/filesystementrymodel.h"
#include "featureobject.h"

class FileSystemEntryObject final : public FeatureObject {
  Q_OBJECT

 public:
  explicit FileSystemEntryObject(FileSystemEntryModel* model);
  explicit FileSystemEntryObject(FileSystemEntryModel* model,
                                 const std::filesystem::path& path,
                                 const QIcon& icon, const QString& input);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  std::filesystem::path path_{};
};
