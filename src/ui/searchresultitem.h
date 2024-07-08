#ifndef SEARCHRESULTITEM_H
#define SEARCHRESULTITEM_H

#include <QListWidget>
#include <QListWidgetItem>
#include <cstdint>

#include "../core/id.h"

class SearchResultItem : public QListWidgetItem {
 public:
  explicit SearchResultItem(const Id& id, QListWidget* parent = nullptr);

  Id GetId() const;

 private:
  Id id_;
};

#endif
