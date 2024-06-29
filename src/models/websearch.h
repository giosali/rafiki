#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QJsonObject>
#include <QKeyCombination>
#include <QString>

#include "result.h"

class WebSearch : public Result {
  Q_OBJECT

 public:
  explicit WebSearch(const QJsonObject& object);

  bool IsCustom() const;

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  QString alt_url_;
  bool is_custom_;
  QString url_;
};

#endif  // WEBSEARCH_H
