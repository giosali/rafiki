#ifndef URL_H
#define URL_H

#include <cstdint>

#include "processedresult.h"

class Url : public ProcessedResult {
  Q_OBJECT

 public:
  Url();

  bool ProcessInput(const Input& input) override;

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;
};

#endif  // URL_H
