#ifndef PROCESSEDRESULTBUILDER_H
#define PROCESSEDRESULTBUILDER_H

#include <memory>
#include <vector>

#include "processedresult.h"

class ProcessedResultBuilder : public ProcessedResult {
 public:
  using ProcessedResult::ProcessedResult;

  std::vector<std::shared_ptr<ProcessedResult>> GetResults() const;

 protected:
  std::vector<std::shared_ptr<ProcessedResult>> results_;
};

#endif  // PROCESSEDRESULTBUILDER_H
