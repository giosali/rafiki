#include "searchresultitem.h"

SearchResultItem::SearchResultItem(uint64_t id, QListWidget* parent)
    : QListWidgetItem{parent}, id_{id} {}

uint64_t SearchResultItem::GetId() const { return id_; }
