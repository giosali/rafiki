#include "searchresultlist.h"

#include <QDebug>
#include <QFrame>
#include <QListWidgetItem>
#include <algorithm>
#include <iterator>
#include <memory>

#include "searchresult.h"

SearchResultList::SearchResultList(QWidget* parent)
    : QListWidget(parent), project_io_() {
  // Component should be hidden on initialization.
  hide();

  // Removes thin border around component.
  setFrameStyle(QFrame::NoFrame);
}

int SearchResultList::Height() const {
  auto row_height = sizeHintForRow(0);
  auto min_num_rows = std::min(count(), kMaxCount);
  auto total_height = min_num_rows * row_height;
  qDebug() << "SearchResultList::Height() = " << total_height;
  return total_height;
}

void SearchResultList::ProcessInput(const QString& input) {
  clear();

  if (input.length() == 0) {
    hide();
    emit ItemsCleared(this);
    return;
  }

  auto models = project_io_.FindDataModels(input);
  for (std::move_iterator it{models.begin()}, end{models.end()}; it != end;
       ++it) {
    auto value = *it;
    AddItem(value->GetIcon(), value->GetTitle(input), value->GetDescription());
  }

  if (count() == 0) {
    // TODO: add default results to list.
    hide();
    emit ItemsCleared(this);
    return;
  }

  show();
  emit ItemsAdded(this);
}

void SearchResultList::AddItem(const QString& icon, const QString& title,
                               const QString& description) {
  auto widget = std::make_unique<SearchResult>(icon, title, description, this);

  auto item = std::make_unique<QListWidgetItem>(this);
  item->setSizeHint(widget->sizeHint());

  addItem(item.get());
  setItemWidget(item.release(), widget.release());
}
