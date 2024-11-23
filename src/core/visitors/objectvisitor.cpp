#include "objectvisitor.h"

ObjectVisitor::ObjectVisitor(const QString& input) : input_{input} {}

void ObjectVisitor::Visit(ApplicationModel* model) {
  auto bridge = model->GetFeatureBridge();
  auto objects = bridge->ProcessInput(model, input_);
  feature_objects_.insert(feature_objects_.end(), objects.begin(),
                          objects.end());
}

void ObjectVisitor::Visit(CalculatorModel* model) {
  auto bridge = model->GetFeatureBridge();
  auto objects = bridge->ProcessInput(model, input_);
  feature_objects_.insert(feature_objects_.end(), objects.begin(),
                          objects.end());
}

void ObjectVisitor::Visit(FileSystemEntryModel* model) {
  auto bridge = model->GetFeatureBridge();
  auto objects = bridge->ProcessInput(model, input_);
  feature_objects_.insert(feature_objects_.end(), objects.begin(),
                          objects.end());
}

void ObjectVisitor::Visit(TrashModel* model) {
  auto bridge = model->GetFeatureBridge();
  auto objects = bridge->ProcessInput(model, input_);
  feature_objects_.insert(feature_objects_.end(), objects.begin(),
                          objects.end());
}

void ObjectVisitor::Visit(UrlModel* model) {
  auto bridge = model->GetFeatureBridge();
  auto objects = bridge->ProcessInput(model, input_);
  feature_objects_.insert(feature_objects_.end(), objects.begin(),
                          objects.end());
}

void ObjectVisitor::Visit(WebSearchModel* model) {
  auto bridge = model->GetFeatureBridge();
  auto objects = no_parse_ ? bridge->PassInput(model, input_)
                           : bridge->ProcessInput(model, input_);
  feature_objects_.insert(feature_objects_.end(), objects.begin(),
                          objects.end());
}

std::vector<FeatureObject*> ObjectVisitor::GetFeatureObjects() {
  return feature_objects_;
}

void ObjectVisitor::SetNoParse(bool value) { no_parse_ = value; }
