#ifndef PROCESSEDRESULT_H
#define PROCESSEDRESULT_H

#include "../core/input.h"
#include "baseresult.h"

class ProcessedResult : public BaseResult {
  Q_OBJECT

 public:
  using BaseResult::BaseResult;

  virtual bool ProcessInput(const Input& input) = 0;
};

#endif  // PROCESSEDRESULT_H
