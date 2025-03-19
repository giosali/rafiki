#include "refreshobject.h"

#include "../indexer.h"

RefreshObject::RefreshObject(FeatureModel* model) : FeatureObject{model} {}

void RefreshObject::Drag() {}

void RefreshObject::ProcessKeyPress(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Tab:
      FeatureObject::ProcessKeyPress(combination);
      break;
    case Qt::Key_Return: {
      emit Hidden();

      // This must be called prior to clearing and initializing the indexer.
      FeatureObject::ProcessKeyPress(combination);

      Indexer::GetInstance().Reinitialize();
      break;
    }
    default:
      break;
  }
}

void RefreshObject::ProcessKeyRelease(const QKeyCombination& combination) {}
