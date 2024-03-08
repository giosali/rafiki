#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QJsonObject>
#include <QString>

#include "datamodel.h"

class WebSearch : public DataModel {
 public:
  explicit WebSearch() = default;

  ~WebSearch() = default;

  Action AltGo(const QString& arg) override;
  QString GetAltTitle() override;
  Action Go(const QString& arg) override;
  void Populate(const QJsonObject& object) override;

 private:
  Action ProcessUrl(const QString& url, const QString& arg) const;

  QString alt_url_;
  QString alt_title_;
  int id_;
  QString url_;
};

#endif  // WEBSEARCH_H
