#pragma once

#include "featuremodel.h"

class FileSystemEntryModel final : public FeatureModel {
 public:
  FileSystemEntryModel();

  void Accept(FeatureVisitor& v) override;
  QString FormatCommand() const override;
  bool ReceivesInput() const override;
};
