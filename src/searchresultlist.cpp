#include "searchresultlist.h"

#include <QFrame>
#include <QListWidgetItem>
#include <QObject>
#include <algorithm>
#include <iterator>
#include <memory>

#include "searchresult.h"

SearchResultList::SearchResultList(QWidget* parent)
    : QListWidget(parent), project_io_() {
  // Component should be hidden on initialization.
  if (parent == nullptr) {
    setFixedHeight(0);
  } else {
    setFixedSize(parent->width(), 0);
  }

  // Removes thin border around component.
  setFrameStyle(QFrame::NoFrame);

  QObject::connect(this, &SearchResultList::ItemsAdded, this,
                   &SearchResultList::AdjustSize);
  QObject::connect(this, &SearchResultList::ItemsCleared, this,
                   &SearchResultList::AdjustSize);
}

int SearchResultList::Height() const {
  auto row_height = sizeHintForRow(0);
  if (row_height == -1) {
    return 0;
  }

  auto min_num_rows = std::min(count(), kMaxCount);
  auto total_height = min_num_rows * row_height;
  return total_height;
}

void SearchResultList::AdjustSize(SearchResultList* list) {
  setFixedHeight(Height());
}

void SearchResultList::ProcessInput(const QString& input) {
  clear();

  if (input.length() == 0) {
    emit ItemsCleared(this);
    return;
  }

  auto models = project_io_.FindDataModels(input);
  for (const auto& model : models) {
    AddItem(model->GetIcon(), model->GetTitle(input), model->GetDescription());
  }

  if (count() == 0) {
    // TODO: add default results to list.
    emit ItemsCleared(this);
    return;
  }

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
