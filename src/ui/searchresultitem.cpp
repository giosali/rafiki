#include "searchresultitem.h"

SearchResultItem::SearchResultItem(const QUuid& id, QListWidget* parent)
    : QListWidgetItem{parent}, id_{id} {}

QUuid SearchResultItem::GetId() const { return id_; }
