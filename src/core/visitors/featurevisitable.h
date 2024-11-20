#pragma once

class FeatureVisitor;

class FeatureVisitable {
 public:
  virtual void Accept(FeatureVisitor& v) = 0;
};
