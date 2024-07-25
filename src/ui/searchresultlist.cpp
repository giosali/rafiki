#include "searchresultlist.h"

#include <QApplication>
#include <QCursor>
#include <QFrame>
#include <QIcon>
#include <QList>
#include <QScrollBar>
#include <Qt>
#include <QtGlobal>
#include <algorithm>
#include <cstdlib>

#include "../core/config.h"
#include "../core/crud.h"
#include "searchresultitem.h"

SearchResultList::SearchResultList(SearchBox* search_box, MainWindow* parent)
    : QListWidget{parent}, main_window_{parent}, search_box_{search_box} {
  // Component should be hidden on initialization.
  parent == nullptr ? setFixedHeight(0) : setFixedSize(parent->width(), 0);

  // Removes thin border around component.
  setFrameStyle(QFrame::NoFrame);

  setFocusPolicy(Qt::NoFocus);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Styles the scrollbar.
  // https://doc.qt.io/qt-6/stylesheet-examples.html#customizing-qscrollbar
  auto stylesheet = QString{
    "QScrollBar:vertical { background: none; border: none; "
    "margin: 0px; "
    "width: 4px; } QScrollBar::handle:vertical { background: gray; } "
    "QScrollBar::add-line:vertical { "
    "background: "
    "none; border: none; } QScrollBar::sub-line:vertical { background: "
    "none; border: none; }"};
  setStyleSheet(stylesheet);

  // This is required to properly enable mouse events.
  setMouseTracking(true);

  // Inherited -> this
  connect(this, &QListWidget::itemClicked, this,
          &SearchResultList::ActivateItem);
  connect(this, &QListWidget::currentItemChanged, this,
          &SearchResultList::CheckSelectedItem);

  // this -> this
  connect(this, &SearchResultList::ItemsChanged, this,
          &SearchResultList::AdjustSize);
}

SearchResultList::~SearchResultList() {
  worker_thread_.quit();
  worker_thread_.wait();
}

void SearchResultList::ActivateItem(QListWidgetItem* item) {
  if (starting_drag_position_ == mapFromGlobal(QCursor::pos())) {
    emit KeyPressReceived(QKeyCombination{Qt::Key_Return});
  }
}

void SearchResultList::AdjustSize(int height) { setFixedHeight(height); }

void SearchResultList::CheckSelectedItem(QListWidgetItem* current,
                                         QListWidgetItem* previous) {
  // Ensures an item will always be selected.
  if (current == nullptr) {
    if (previous != nullptr) {
      setCurrentItem(previous);
    } else if (count() > 0) {
      setCurrentRow(0);
    }
  }
}

void SearchResultList::ProcessInput(const Input& input) {
  // This is necessary for when CPU intensive operations are currently underway.
  // Most of the time, this will be redundant.
  worker_thread_.exit();

  entered_ = false;

  if (input.IsEmpty()) {
    clear();                      // Helps prevent flicker.
    user_selected_item_ = false;  // Resets.
    emit ItemsChanged(Height());
    return;
  }

  // Does not work with smart pointers.
  auto worker = new searchresultlist::Worker{};

  worker->moveToThread(&worker_thread_);
  connect(&worker_thread_, &QThread::finished, worker, &QObject::deleteLater);
  connect(this, &SearchResultList::InputReceived, worker,
          &searchresultlist::Worker::ProcessInput);
  connect(worker, &searchresultlist::Worker::ResultsReadied, this,
          &SearchResultList::ProcessResults);
  worker_thread_.start();

  emit InputReceived(input);
}

void SearchResultList::ProcessKeyPress(const QKeyCombination& combination) {
  auto key = combination.key();
  switch (key) {
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
      if (combination.keyboardModifiers() & Qt::ControlModifier &&
          key - Qt::Key_0 <= count()) {
        auto row = key - Qt::Key_1 + verticalScrollBar()->value();
        setCurrentRow(row);
        emit KeyPressReceived(QKeyCombination{Qt::Key_Return});
      }

      break;
    case Qt::Key_Up:
    case Qt::Key_Down:
      if (auto row = currentRow() + (key - Qt::Key_Right);
          row >= 0 && row < count()) {
        setCurrentRow(row);
        user_selected_item_ = true;
      }

      break;
    default:
      emit KeyPressReceived(combination);
      break;
  }
}

void SearchResultList::ProcessKeyRelease(const QKeyCombination& combination) {
  emit KeyReleaseReceived(combination);
}

void SearchResultList::ProcessResults(
  const std::vector<std::shared_ptr<Result>>& results, const Input& input,
  const QString& argument, bool set_row_to_zero) {
  if (set_row_to_zero) {
    user_selected_item_ = false;
  }

  auto row = 0;
  if (auto current_row = currentRow();
      current_row == -1 || !user_selected_item_) {
    clear();  // Helps prevent flicker.

    for (size_t i = 0; i < results.size(); ++i) {
      AddItem(results[i], input, argument, i);
    }
  } else {
    auto current_id =
      static_cast<SearchResultItem*>(item(current_row))->GetId();

    clear();  // Helps prevent flicker.

    // Ensures we don't check for matching IDs since there can only be one
    // match.
    bool found_id = false;

    for (size_t i = 0; i < results.size(); ++i) {
      auto result = results[i];
      AddItem(result, input, argument, i);

      if (!found_id && result->GetId() == current_id) {
        row = i;
        found_id = true;
      }
    }
  }

  setCurrentRow(row);
  emit ItemsChanged(Height());

  // Worker thread is no longer needed at this point.
  worker_thread_.exit();
}

void SearchResultList::SetUserSelectedItem(bool value) {
  user_selected_item_ = value;
}

void SearchResultList::mouseMoveEvent(QMouseEvent* event) {
  // Prevents an item from being selected when items are rendered and the cursor
  // happens to be hovering over the newly rendered items.
  if (!entered_) {
    entered_ = true;
    return;
  }

  // Selects item currently under mouse.
  auto position = event->position();
  if (auto item = itemAt(position.x(), position.y());
      item != nullptr && !item->isSelected()) {
    setCurrentItem(item);
    user_selected_item_ = true;
  }

  // Exits if left mouse button isn't down while dragging.
  if (!(event->buttons() & Qt::LeftButton)) {
    QListWidget::mouseMoveEvent(event);
    return;
  }

  // Exits if the distance traveled is less than the recommended drag distance
  // to start a drop and drag operation.
  auto drag_distance = (position - starting_drag_position_).manhattanLength();
  if (drag_distance < QApplication::startDragDistance()) {
    return;
  }

  emit ItemDragged();
}

void SearchResultList::mousePressEvent(QMouseEvent* event) {
  switch (event->button()) {
    case Qt::LeftButton:
      starting_drag_position_ = event->position();

      // Prevents user from deselecting an item by control-clicking it.
      if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        return;
      }

      break;
  }

  QListWidget::mousePressEvent(event);
}

void SearchResultList::AddItem(const std::shared_ptr<Result>& result,
                               const Input& input, const QString& argument,
                               int index) {
  auto widget = new SearchResult(result, input, argument, index, this);
  connect(verticalScrollBar(), &QScrollBar::valueChanged, widget,
          &SearchResult::UpdateShortcut);
  connect(this, &QListWidget::currentRowChanged, widget,
          &SearchResult::SetIsSelected);
  connect(this, &SearchResultList::ItemDragged, widget, &SearchResult::Drag);
  connect(this, &SearchResultList::KeyPressReceived, widget,
          &SearchResult::ProcessKeyPress);
  connect(this, &SearchResultList::KeyReleaseReceived, widget,
          &SearchResult::ProcessKeyRelease);

  auto interactable = result.get();
  connect(interactable, &Interactable::NewSearchBoxTextRequested, search_box_,
          &SearchBox::SetText);
  connect(interactable, &Interactable::Hidden, main_window_, &MainWindow::Hide);

  auto item = new SearchResultItem(result->GetId(), this);

  // Sets the actual height of search result items and prevents unusual sizing
  // differences between items.
  item->setSizeHint(widget->sizeHint());

  setItemWidget(item, widget);
}

int SearchResultList::Height() const {
  auto row_height = sizeHintForRow(0);
  if (row_height == -1) {
    return 0;
  }

  auto min_num_rows = std::min(count(), Config::search_result_list_max_count_);
  auto total_height = min_num_rows * row_height;
  return total_height;
}

namespace searchresultlist {
void Worker::ProcessInput(const Input& input) {
  // When the user:
  // - ***is just now*** receiving default search results
  // - deletes all input
  // --> *Don't* reselect the previously selected item. Select the first item.
  // When the user:
  // - manually selects an item (through arrow keys/mouse hover)
  // --> Reselect the previously selected item.
  static bool last_results_were_defaults = false;

  auto results = Crud::ReadResults(input);
  if (results.empty()) {
    emit ResultsReadied(Crud::ReadDefaultResults(), input, input.ToString(),
                        !last_results_were_defaults);
    last_results_were_defaults = true;
  } else {
    emit ResultsReadied(results, input, input.Argument(),
                        last_results_were_defaults);
    last_results_were_defaults = false;
  }
}
}  // namespace searchresultlist
