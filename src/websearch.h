#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QJsonObject>
#include <QString>
#include <QUuid>
#include <QWidget>

#include "datamodel.h"
#include "definitions.h"
#include "searchresultlist.h"

class WebSearch : public DataModel {
 public:
  explicit WebSearch() = default;

  ~WebSearch() = default;

  QString DragAndDrop() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       QWidget* parent) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         QWidget* parent) override;
  defs::Action AltGo(const QString& arg) override;
  QString GetAltTitle() override;
  defs::Action Go(const QString& arg) override;
  void Populate(const QJsonObject& object) override;

 private:
  void ProcessUrl(const QString& url,
                  SearchResultList* search_result_list) const;

  QString alt_url_;
  QString alt_title_;
  QString url_;
};

#endif  // WEBSEARCH_H
