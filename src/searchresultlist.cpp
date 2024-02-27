#include "searchresultlist.h"

#include <QFrame>
#include <QListWidgetItem>
#include <algorithm>
#include <cstdlib>
#include <memory>

#include "searchresult.h"

SearchResultList::SearchResultList(QWidget* parent) : QListWidget(parent) {
  // Component should be hidden on initialization.
  hide();

  // Removes slight border around component.
  setFrameStyle(QFrame::NoFrame);
}

int SearchResultList::Height() {
  auto row_height = sizeHintForRow(0);
  auto min_num_rows = std::min(count(), kMaxCount);
  auto total_height = min_num_rows * row_height;
  return total_height;
}

void SearchResultList::AddItem(const QString& icon_path) {
  auto widget = std::make_unique<SearchResult>();
  widget->SetIcon(icon_path);

  auto item = std::make_unique<QListWidgetItem>(this);
  item->setSizeHint(widget->sizeHint());

  addItem(item.get());
  setItemWidget(item.release(), widget.release());
}

void SearchResultList::CreateItems(const QString& text) {
  clear();
  if (text.length() == 0) {
    hide();
    return;
  }

  show();
  auto num_items_to_create = 7;
  for (size_t i = 0; i < num_items_to_create; i++) {
    AddItem("://images/question-mark.png");
  }

  emit ItemsAdded(this);
}
