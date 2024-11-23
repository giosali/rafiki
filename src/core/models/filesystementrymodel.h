#pragma once

#include <QString>

#include "featuremodel.h"

class FileSystemEntryModel final : public FeatureModel {
 public:
  FileSystemEntryModel();

  void Accept(FeatureVisitor& v) override;
  bool ReceivesInput() const override;

  static const QString kCommand;
};
