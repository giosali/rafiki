#ifndef SEARCHRESULTITEM_H
#define SEARCHRESULTITEM_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QUuid>

class SearchResultItem : public QListWidgetItem {
 public:
  explicit SearchResultItem(const QUuid& id, QListWidget* parent = nullptr);

  QUuid GetId() const;

 private:
  QUuid id_;
};

#endif
