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
  parent == nullptr ? setFixedHeight(0) : setFixedSize(parent->width(), 0);

  // Removes thin border around component.
  setFrameStyle(QFrame::NoFrame);

  setFocusPolicy(Qt::NoFocus);

  QObject::connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
                   &SearchResultList::UpdateShortcuts);
  QObject::connect(this, &SearchResultList::ItemsChanged, this,
                   &SearchResultList::AdjustSize);
  QObject::connect(this, &SearchResultList::ItemsChanged, this,
                   &SearchResultList::SetCurrentItem);
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
    emit ItemsChanged(this);
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

  arg_ = arg;
  auto models = project_io_.FindDataModels(cmd);
  for (size_t i = 0; i < models.size(); ++i) {
    AddItem(models[i], arg, i);
  }

  if (count() == 0) {
    // TODO: add default results to list.
    emit ItemsChanged(this);
    return;
  }

  emit ItemsChanged(this);
}

void SearchResultList::ProcessKeyPress(const QKeyCombination& key_combination) {
  auto key = key_combination.key();
  auto modifiers = key_combination.keyboardModifiers();
  switch (key) {
    case Qt::Key_Tab: {
      auto search_result = SearchResultAt(currentRow());
      if (search_result == nullptr) {
        break;
      }

      emit SetTextRequested(search_result->GetCommand());
      break;
    }
    case Qt::Key_Return: {
      auto search_result = SearchResultAt(currentRow());
      if (search_result == nullptr) {
        break;
      }

      if (modifiers & Qt::AltModifier) {
        ProcessAction(search_result->PressAltReturn(), search_result);
        break;
      }

      ProcessAction(search_result->PressReturn(arg_), search_result);
      break;
    }
    case Qt::Key_Up: {
      auto new_current_row = currentRow() - 1;
      if (new_current_row < 0) {
        break;
      }

      setCurrentRow(new_current_row);
      break;
    }
    case Qt::Key_Down: {
      auto new_current_row = currentRow() + 1;
      if (new_current_row >= count()) {
        break;
      }

      setCurrentRow(new_current_row);
      break;
    }
    case Qt::Key_Alt: {
      auto search_result = SearchResultAt(currentRow());
      if (search_result == nullptr) {
        break;
      }

      search_result->PressAlt();
      break;
    }
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6: {
      if (modifiers & Qt::ControlModifier) {
        auto row = key - Qt::Key_1 + verticalScrollBar()->value();
        auto search_result = SearchResultAt(row);
        if (search_result == nullptr) {
          break;
        }

        ProcessAction(search_result->PressReturn(arg_), search_result);
      }

      break;
    }
  }
}

void SearchResultList::ProcessKeyRelease(
    const QKeyCombination& key_combination) {
  auto key = key_combination.key();
  switch (key) {
    case Qt::Key_Alt: {
      auto search_result = SearchResultAt(currentRow());
      if (search_result == nullptr) {
        break;
      }

      search_result->ReleaseAlt(arg_);
      break;
    }
  }
}

void SearchResultList::SetCurrentItem(SearchResultList* list) {
  setCurrentRow(0);
}

void SearchResultList::UpdateShortcuts(int value) {
  for (size_t i = 0, j = 0; i < count() && j < kMaxCount; ++i) {
    if (i < value) {
      continue;
    }

    auto search_result = SearchResultAt(i);
    if (search_result == nullptr) {
      continue;
    }

    search_result->SetShortcut(QString::number(++j));
  }
}

void SearchResultList::AddItem(const std::shared_ptr<DataModel>& data_model,
                               const QString& arg, int row) {
  auto key = row < kMaxCount ? QString::number(row + 1) : (const char*)0;
  auto widget = std::make_unique<SearchResult>(data_model, arg, key, this);
  auto item = std::make_unique<QListWidgetItem>(this);

  // Sets the actual height of search result items and prevents unusual sizing
  // differences between items.
  item->setSizeHint(widget->sizeHint());

  setItemWidget(item.release(), widget.release());
}

void SearchResultList::ProcessAction(defs::Action action,
                                     SearchResult* search_result) {
  switch (action) {
    case defs::Action::HideWindow:
      emit HideWindowRequested();
      break;
    case defs::Action::Nothing:
      break;
    case defs::Action::SetTextToCommand:
      emit SetTextRequested(search_result->GetCommand());
      break;
  }
}

SearchResult* SearchResultList::SearchResultAt(int row) {
  auto list_item = item(row);
  if (list_item == nullptr) {
    return nullptr;
  }

  auto widget = itemWidget(list_item);
  return static_cast<SearchResult*>(widget);
}
