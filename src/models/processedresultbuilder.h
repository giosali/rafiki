#ifndef PROCESSEDRESULTBUILDER_H
#define PROCESSEDRESULTBUILDER_H

#include <vector>

#include "processedresult.h"

class ProcessedResultBuilder : public ProcessedResult {
 public:
  using ProcessedResult::ProcessedResult;

  std::vector<ProcessedResult> GetResults() const;

 protected:
  std::vector<ProcessedResult> processed_results_;
};

#endif  // PROCESSEDRESULTBUILDER_H
