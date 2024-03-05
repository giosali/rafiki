#include "searchresultlist.h"

#include <QFrame>
#include <QListWidgetItem>
#include <QObject>
#include <QScrollBar>
#include <algorithm>
#include <cstdlib>
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
  QObject::connect(this, &SearchResultList::ItemsAdded, this,
                   &SearchResultList::SetCurrentItem);
  QObject::connect(this, &SearchResultList::ItemsCleared, this,
                   &SearchResultList::AdjustSize);
  QObject::connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
                   &SearchResultList::UpdateShortcuts);
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
  size_t index = 0;
  for (const auto& model : models) {
    auto shortcut_key =
        index < kMaxCount ? QString::number(++index) : (const char*)0;
    AddItem(model->GetIcon(), model->GetTitle(input), model->GetDescription(),
            shortcut_key);
  }

  if (count() == 0) {
    // TODO: add default results to list.
    emit ItemsCleared(this);
    return;
  }

  emit ItemsAdded(this);
}

void SearchResultList::SetCurrentItem(SearchResultList* list) {
  auto first_item = item(0);
  if (first_item == nullptr) {
    return;
  }

  setCurrentItem(first_item);
}

void SearchResultList::UpdateShortcuts(int value) {
  for (size_t i = 0, j = 0; i < count() && j < kMaxCount; ++i) {
    if (i < value) {
      continue;
    }

    auto list_item = item(i);
    auto widget = itemWidget(list_item);
    auto search_result = dynamic_cast<SearchResult*>(widget);
    if (search_result == nullptr) {
      continue;
    }

    search_result->SetShortcut(QString::number(++j));
  }
}

void SearchResultList::AddItem(const QString& icon, const QString& title,
                               const QString& description,
                               const QString& shortcut_key) {
  auto widget = std::make_unique<SearchResult>(icon, title, description,
                                               shortcut_key, this);

  auto item = std::make_unique<QListWidgetItem>(this);
  item->setSizeHint(widget->sizeHint());

  addItem(item.get());
  setItemWidget(item.release(), widget.release());
}
