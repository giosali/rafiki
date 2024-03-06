#include "searchresultlist.h"

#include <QFrame>
#include <QListWidgetItem>
#include <QObject>
#include <QScrollBar>
#include <QSize>
#include <Qt>
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

  auto cmd = input;
  auto arg = QString();
  auto space_index = input.indexOf(" ");

  // Increments in order to include the space when slicing.
  if (space_index++ != -1) {
    cmd = input.sliced(0, space_index);
    arg = input.sliced(space_index);
  }

  auto models = project_io_.FindDataModels(cmd);
  for (size_t i = 0; i < models.size(); ++i) {
    AddItem(models[i], arg, i);
  }

  if (count() == 0) {
    // TODO: add default results to list.
    emit ItemsCleared(this);
    return;
  }

  emit ItemsAdded(this);
}

void SearchResultList::ProcessKeyPress(int key) {
  switch (key) {
    case Qt::Key::Key_Up: {
      auto new_current_row = currentRow() - 1;
      if (new_current_row < 0) {
        break;
      }

      setCurrentRow(new_current_row);
      break;
    }
    case Qt::Key::Key_Down: {
      auto new_current_row = currentRow() + 1;
      if (new_current_row >= count()) {
        break;
      }

      setCurrentRow(new_current_row);
      break;
    }
  }
}

void SearchResultList::SetCurrentItem(SearchResultList* list) {
  setCurrentRow(0);
}

void SearchResultList::AddItem(std::shared_ptr<DataModel> data_model,
                               const QString& arg, int row) {
  auto widget = std::make_unique<SearchResult>(data_model, arg, row, this);
  QObject::connect(verticalScrollBar(), SIGNAL(valueChanged(int)), widget.get(),
                   SLOT(SetShortcut(int)));

  auto item = std::make_unique<QListWidgetItem>(this);

  // Sets the actual height of search result items and prevents unusual sizing
  // differences between items.
  auto size_hint = QSize(widget->sizeHint().width(), widget->Height());
  item->setSizeHint(size_hint);

  addItem(item.get());
  setItemWidget(item.release(), widget.release());
}
