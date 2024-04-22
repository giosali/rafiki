#include "processedresultbuilder.h"

std::vector<std::shared_ptr<ProcessedResult>>
ProcessedResultBuilder::GetResults() const {
  return results_;
}
