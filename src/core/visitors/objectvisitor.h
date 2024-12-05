#pragma once

#include <vector>

#include "../bridges/featurebridge.h"
#include "../objects/featureobject.h"
#include "featurevisitor.h"

class ObjectVisitor final : public FeatureVisitor {
 public:
  explicit ObjectVisitor(const QString& input);

  void Visit(ApplicationModel* model) override;
  void Visit(CalculatorModel* model) override;
  void Visit(FileSystemEntryModel* model) override;
  void Visit(SettingsModel* model) override;
  void Visit(TrashModel* model) override;
  void Visit(UrlModel* model) override;
  void Visit(WebSearchModel* model) override;
  std::vector<FeatureObject*> GetFeatureObjects();
  void SetNoParse(bool value);

 private:
  std::vector<FeatureObject*> feature_objects_{};
  QString input_{};
  bool no_parse_{false};
};
