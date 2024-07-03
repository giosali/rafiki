#ifndef PROCESSEDRESULT_H
#define PROCESSEDRESULT_H

#include "../core/input.h"
#include "result.h"

class ProcessedResult : public Result {
  Q_OBJECT

 public:
  virtual bool ProcessInput(const Input& input) = 0;
};

#endif  // PROCESSEDRESULT_H
