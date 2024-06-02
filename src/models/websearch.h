#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QJsonObject>
#include <QKeyCombination>
#include <QString>

#include "baseresult.h"

class WebSearch : public BaseResult {
  Q_OBJECT

 public:
  explicit WebSearch(const QJsonObject& object);

  ~WebSearch() = default;

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  QString alt_url_;
  QString url_;
};

#endif  // WEBSEARCH_H
