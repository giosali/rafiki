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

 private:
  static const QString kAltTitle;
  static const bool kAppendSpaceToCommand;
  static const QString kCommand;
  static const QString kDescription;
  static const QString kIcon;
  static const uint64_t kId;
  static const QString kInfoDescription;
  static const QString kTitle;
  static const QString kTitlePlaceholder;
};

#endif  // URL_H
