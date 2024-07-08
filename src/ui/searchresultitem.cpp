#include "searchresultitem.h"

SearchResultItem::SearchResultItem(const Id& id, QListWidget* parent)
    : QListWidgetItem{parent}, id_{id} {}

Id SearchResultItem::GetId() const { return id_; }
