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

#include "../core/models/featuremodel.h"
#include "../core/visitors/objectvisitor.h"
#include "searchresult.h"
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

void SearchResultList::ProcessText(const QString& text) {
  // This is necessary for when CPU intensive operations are currently underway.
  // Most of the time, this will be redundant.
  worker_thread_.exit();

  entered_ = false;

  if (text.isEmpty()) {
    clear();                      // Helps prevent flicker.
    user_selected_item_ = false;  // Resets.
    emit ItemsChanged(Height());
    return;
  }

  // Does not work with smart pointers.
  auto worker = new searchresultlist::Worker{};

  worker->moveToThread(&worker_thread_);
  connect(&worker_thread_, &QThread::finished, worker, &QObject::deleteLater);
  connect(this, &SearchResultList::TextReceived, worker,
          &searchresultlist::Worker::ProcessText);
  connect(worker, &searchresultlist::Worker::ObjectsReadied, this,
          &SearchResultList::ProcessObjects);
  worker_thread_.start();

  emit TextReceived(text);
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

void SearchResultList::ProcessObjects(std::vector<FeatureObject*> objects,
                                      const QString& text,
                                      bool set_row_to_zero) {
  if (set_row_to_zero) {
    user_selected_item_ = false;
  }

  auto row = 0;
  if (auto current_row = currentRow();
      current_row == -1 || !user_selected_item_) {
    clear();  // Helps prevent flicker.

    for (size_t i = 0; i < objects.size(); ++i) {
      AddItem(objects[i], text, i);
    }
  } else {
    auto current_id =
      static_cast<SearchResultItem*>(item(current_row))->GetId();

    clear();  // Helps prevent flicker.

    // Ensures we don't check for matching IDs since there can only be one
    // match.
    bool found_id = false;

    for (size_t i = 0; i < objects.size(); ++i) {
      auto object = objects[i];
      AddItem(object, text, i);

      if (!found_id && object->GetId() == current_id) {
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
    default:
      break;
  }

  QListWidget::mousePressEvent(event);
}

void SearchResultList::AddItem(FeatureObject* object, const QString& text,
                               int index) {
  auto widget = new SearchResult{object, text, index, this};

  connect(verticalScrollBar(), &QScrollBar::valueChanged, widget,
          &SearchResult::UpdateShortcut);
  connect(this, &QListWidget::currentRowChanged, widget,
          &SearchResult::SetIsSelected);
  connect(this, &SearchResultList::ItemDragged, widget, &SearchResult::Drag);
  connect(this, &SearchResultList::KeyPressReceived, widget,
          &SearchResult::ProcessKeyPress);
  connect(this, &SearchResultList::KeyReleaseReceived, widget,
          &SearchResult::ProcessKeyRelease);

  connect(object, &FeatureObject::NewSearchBoxTextRequested, search_box_,
          &SearchBox::SetText);
  connect(object, &FeatureObject::Hidden, main_window_, &MainWindow::Hide);

  auto item = new SearchResultItem(object->GetId(), this);

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

  auto min_num_rows =
    std::min(count(), Settings::GetInstance().GetSearchResultListMaxCount());
  auto total_height = min_num_rows * row_height;
  return total_height;
}

namespace searchresultlist {
void Worker::ProcessText(const QString& text) {
  // When the user:
  // - ***is just now*** receiving default search results
  // - deletes all input
  // --> *Don't* reselect the previously selected item. Select the first item.
  // When the user:
  // - manually selects an item (through arrow keys/mouse hover)
  // --> Reselect the previously selected item.
  static bool last_results_were_defaults = false;

  // Handles query processing.
  auto visitor = ObjectVisitor{text};
  for (auto id : indexer_.GetIds(text)) {
    auto model = indexer_.GetModel(id);
    if (model->GetIsEnabled()) {
      model->Accept(visitor);
    }
  }

  auto objects = visitor.GetFeatureObjects();

  // Handles the results of query processing.
  if (objects.empty()) {
    visitor.SetNoParse(true);
    const auto default_ids = settings_.GetDefaultFeatureModelIds();
    for (size_t i = 0; i < default_ids.size(); ++i) {
      auto model = indexer_.GetModel(default_ids[i]);
      model->Accept(visitor);
    }

    objects = visitor.GetFeatureObjects();

    // TODO: find more efficient way of moving QObject instead of looping.
    for (const auto& i : objects) {
      i->moveToThread(QApplication::instance()->thread());
    }

    // `results` are default results
    emit ObjectsReadied(objects, text, !last_results_were_defaults);
    last_results_were_defaults = true;
  } else {
    // TODO: find more efficient way of moving QObject instead of looping.
    for (const auto& i : objects) {
      i->moveToThread(QApplication::instance()->thread());
    }

    emit ObjectsReadied(objects, text, last_results_were_defaults);
    last_results_were_defaults = false;
  }
}
}  // namespace searchresultlist
