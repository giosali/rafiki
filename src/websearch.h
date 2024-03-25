#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QJsonObject>
#include <QString>
#include <QWidget>

#include "baseresult.h"
#include "searchresultlist.h"

class WebSearch : public BaseResult {
 public:
  explicit WebSearch(const QJsonObject& object);

  ~WebSearch() = default;

  QString DragAndDrop() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       QWidget* parent) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         QWidget* parent) override;

 private:
  void ProcessUrl(const QString& url,
                  SearchResultList* search_result_list) const;

  QString alt_url_;
  QString url_;
};

#endif  // WEBSEARCH_H
