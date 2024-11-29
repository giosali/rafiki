#pragma once

#include <QCoreApplication>
#include <QString>

#include "featuremodel.h"

class FileSystemEntryModel final : public FeatureModel {
  Q_DECLARE_TR_FUNCTIONS(FileSystemEntryModel)

 public:
  FileSystemEntryModel();

  void Accept(FeatureVisitor& v) override;
  bool ReceivesInput() const override;

  static const QString kCommand;
};
