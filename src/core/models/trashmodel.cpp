#include "trashmodel.h"

#include <Qt>
#include <memory>

#include "../bridges/trashbridge.h"
#include "../paths.h"
#include "../visitors/featurevisitor.h"

TrashModel::TrashModel() : FeatureModel{std::make_unique<TrashBridge>()} {
  SetCommand("empty trash");
  SetIcon(Paths::GetPath(Paths::Image::kTrash));
  SetId(18);
  SetTitle(tr("Empty trash"));
}

void TrashModel::Accept(FeatureVisitor& v) { v.Visit(this); }

bool TrashModel::ReceivesInput() const { return false; }

std::unordered_set<std::string> TrashModel::Tokenize() const {
  auto tokens = FeatureModel::Tokenize();

  auto parts = GetCommand().split(" ", Qt::SkipEmptyParts);
  for (const auto& part : parts) {
    tokens.insert(part.toLower().toStdString());
  }

  return tokens;
}
