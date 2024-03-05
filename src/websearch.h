#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QJsonObject>
#include <QString>

#include "datamodel.h"

class WebSearch : public DataModel {
 public:
  explicit WebSearch() = default;

  ~WebSearch() = default;

  void Populate(const QJsonObject& object) override;

 private:
  QString alt_url_;
  QString alt_title_;
  int id_;
  QString url_;
};

#endif  // WEBSEARCH_H
