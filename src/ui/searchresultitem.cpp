#include "searchresultitem.h"

SearchResultItem::SearchResultItem(uint64_t id, QListWidget* parent)
    : QListWidgetItem{parent}, id_{id} {}

uint64_t SearchResultItem::Id() const { return id_; }
