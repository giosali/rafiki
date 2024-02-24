#include "searchresultlist.h"

#include <QListWidgetItem>
#include <memory>

#include "searchresult.h"

SearchResultList::SearchResultList(QWidget* parent) : QListWidget(parent) {
  // Component should be hidden on initialization.
  hide();
}

void SearchResultList::AddItem(const QString& text) {
  auto item = std::make_unique<QListWidgetItem>();
  auto widget = std::make_unique<SearchResult>();
  widget->SetIcon(text);

  item->setSizeHint(widget->sizeHint());
  addItem(item.get());
  setItemWidget(item.release(), widget.release());
}
