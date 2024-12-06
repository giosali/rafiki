#pragma once

#include "../models/applicationmodel.h"
#include "../models/calculatormodel.h"
#include "../models/filesystementrymodel.h"
#include "../models/refreshmodel.h"
#include "../models/settingsmodel.h"
#include "../models/trashmodel.h"
#include "../models/urlmodel.h"
#include "../models/websearchmodel.h"

class FeatureVisitor {
 public:
  virtual void Visit(ApplicationModel* model) = 0;
  virtual void Visit(CalculatorModel* model) = 0;
  virtual void Visit(FileSystemEntryModel* model) = 0;
  virtual void Visit(RefreshModel* model) = 0;
  virtual void Visit(SettingsModel* model) = 0;
  virtual void Visit(TrashModel* model) = 0;
  virtual void Visit(UrlModel* model) = 0;
  virtual void Visit(WebSearchModel* model) = 0;
};
