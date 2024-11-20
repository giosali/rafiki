#pragma once

#include <filesystem>

#include "../models/filesystementrymodel.h"
#include "featureobject.h"

class FileSystemEntryObject final : public FeatureObject {
  Q_OBJECT

 public:
  explicit FileSystemEntryObject(const FileSystemEntryModel* model);
  explicit FileSystemEntryObject(const FileSystemEntryModel* model,
                                 const std::filesystem::path& path);

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination) override;
  void ProcessKeyRelease(const QKeyCombination& combination) override;

 private:
  const FileSystemEntryModel* model_{nullptr};
  std::filesystem::path path_{};
};
