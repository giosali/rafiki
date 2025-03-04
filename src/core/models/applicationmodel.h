#pragma once

#include <QCoreApplication>
#include <QLocale>
#include <filesystem>
#include <vector>

#include "INIReader.h"
#include "featuremodel.h"

class ApplicationModel final : public FeatureModel {
  Q_DECLARE_TR_FUNCTIONS(ApplicationModel)

 public:
  explicit ApplicationModel(const std::filesystem::path& desktop_entry_path,
                            const INIReader& reader, const QLocale& locale);

  void Accept(FeatureVisitor& v) override;
  bool ReceivesInput() const override;
  std::unordered_set<std::string> Tokenize() const override;
  QString GetExec() const;

 private:
  QString exec_;
  std::vector<QString> keywords_;
};
