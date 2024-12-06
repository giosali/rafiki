#include "refreshmodel.h"

#include "../bridges/refreshbridge.h"
#include "../paths.h"
#include "../visitors/featurevisitor.h"

RefreshModel::RefreshModel() : FeatureModel{std::make_unique<RefreshBridge>()} {
  SetCommand("refresh");
  SetIcon(Paths::GetPath(Paths::Image::kRefresh));
  SetId(21);
  SetTitle(tr("Refresh Rafiki"));
}

void RefreshModel::Accept(FeatureVisitor& v) { v.Visit(this); }

bool RefreshModel::ReceivesInput() const { return false; }
