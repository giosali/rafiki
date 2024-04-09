#include "searchresultlist.h"

#include <QApplication>
#include <QCursor>
#include <QDrag>
#include <QFrame>
#include <QHideEvent>
#include <QIcon>
#include <QKeyEvent>
#include <QList>
#include <QMimeData>
#include <QObject>
#include <QScrollBar>
#include <QSize>
#include <QUrl>
#include <Qt>
#include <algorithm>
#include <cstdlib>

#include "../core/project.h"

SearchResultList::SearchResultList(QWidget* parent) : QListWidget{parent} {
  // Component should be hidden on initialization.
  parent == nullptr ? setFixedHeight(0) : setFixedSize(parent->width(), 0);

  // Removes thin border around component.
  setFrameStyle(QFrame::NoFrame);

  setFocusPolicy(Qt::NoFocus);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // This is required to properly enable mouse events.
  setMouseTracking(true);

  QObject::connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
                   &SearchResultList::UpdateShortcuts);
  QObject::connect(this, &QListWidget::itemClicked, this,
                   &SearchResultList::ActivateItem);
  QObject::connect(this, &SearchResultList::ItemsChanged, this,
                   &SearchResultList::AdjustSize);
  QObject::connect(this, SIGNAL(itemEntered(QListWidgetItem*)), this,
                   SLOT(SetCurrentItem(QListWidgetItem*)));
  QObject::connect(this, SIGNAL(ItemsChanged(SearchResultList*)), this,
                   SLOT(SetCurrentItem(SearchResultList*)));
}

SearchResult* SearchResultList::CurrentSearchResult() const {
  return static_cast<SearchResult*>(itemWidget(currentItem()));
}

QString SearchResultList::GetArg() const { return input_.GetArg(); }

QString SearchResultList::GetCmd() const { return input_.GetCmd(); }

int SearchResultList::Height() const {
  auto row_height = sizeHintForRow(0);
  if (row_height == -1) {
    return 0;
  }

  auto min_num_rows = std::min(count(), kMaxCount);
  auto total_height = min_num_rows * row_height;
  return total_height;
}

void SearchResultList::HideParent() {
  auto event = std::make_unique<QHideEvent>();
  emit EventReceived(event.get());
}

void SearchResultList::ActivateItem(QListWidgetItem* item) {
  if (starting_drag_position_ != mapFromGlobal(QCursor::pos())) {
    return;
  }

  auto search_result = SearchResultAt(row(item));
  if (search_result == nullptr) {
    return;
  }

  auto key_event = std::make_unique<QKeyEvent>(
    QEvent::KeyPress, Qt::Key_Return, QApplication::keyboardModifiers());
  emit EventReceived(key_event.get());
}

void SearchResultList::AdjustSize(SearchResultList* list) {
  setFixedHeight(Height());
}

void SearchResultList::ProcessInput(const Input& input) {
  clear();

  if (input.IsEmpty()) {
    emit ItemsChanged(this);
    return;
  }

  auto base_results = Project::FindBaseResults(input);
  for (size_t i = 0; i < base_results.size(); ++i) {
    AddItem(base_results[i], input.GetArg(), i);
  }

  if (count() == 0) {
    // Adds default search results to list.
    auto default_results = Project::GetDefaultBaseResults();
    for (size_t i = 0; i < default_results.size(); ++i) {
      AddItem(default_results[i], input.GetFull(), i);
    }
  }

  emit ItemsChanged(this);
}

void SearchResultList::ProcessKeyPress(const QKeyCombination& combination) {
  // Prevents manipulation of SearchResult objects that point to nullptr.
  auto current_row = currentRow();
  if (current_row == -1) {
    return;
  }

  auto key = combination.key();

  switch (key) {
    case Qt::Key_Tab:
    case Qt::Key_Return:
    case Qt::Key_Alt: {
      auto search_result = SearchResultAt(current_row);
      search_result->HandleKeyPress(combination, this);
      break;
    }
    case Qt::Key_Up:
    case Qt::Key_Down: {
      // https://doc.qt.io/qt-6/qt.html#Key-enum
      auto new_current_row = current_row + (key - Qt::Key_Right);
      if (new_current_row < 0 || new_current_row >= count()) {
        break;
      }

      setCurrentRow(new_current_row);
      break;
    }
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6: {
      if (combination.keyboardModifiers() & Qt::ControlModifier) {
        // Prevents manipulation of SearchResult objects that point to nullptr.
        if (key - Qt::Key_0 > count()) {
          break;
        }

        auto row = key - Qt::Key_1 + verticalScrollBar()->value();
        auto search_result = SearchResultAt(row);
        search_result->HandleKeyPress(combination, this);
      }

      break;
    }
  }
}

void SearchResultList::ProcessKeyRelease(const QKeyCombination& combination) {
  switch (combination.key()) {
    case Qt::Key_Alt: {
      auto search_result = SearchResultAt(currentRow());
      search_result->HandleKeyRelease(combination, this);
      break;
    }
  }
}

void SearchResultList::SetCurrentItem(QListWidgetItem* item) {
  setCurrentItem(item);
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

void SearchResultList::mouseMoveEvent(QMouseEvent* event) {
  // Exits if left mouse button isn't down while dragging.
  if (!(event->buttons() & Qt::LeftButton)) {
    QListWidget::mouseMoveEvent(event);
    return;
  }

  // Exits if the distance traveled is less than the recommended drag distance
  // to start a drop and drag operation.
  auto current_drag_position = event->pos();
  auto drag_distance =
    (current_drag_position - starting_drag_position_).manhattanLength();
  if (drag_distance < QApplication::startDragDistance()) {
    return;
  }

  auto search_result = SearchResultAt(currentRow());
  if (search_result == nullptr) {
    return;
  }

  // Drag and drop will only apply to local files on user's machine.
  auto url = QUrl::fromLocalFile(search_result->DragAndDrop());
  if (url.isEmpty()) {
    return;
  }

  // Takes the icon from the current search result and uses it as the icon for
  // the drop and drag action.
  auto drag = std::make_unique<QDrag>(this);
  auto icon = QIcon(search_result->GetIcon());
  drag->setPixmap(icon.pixmap(22));

  auto mime_data = std::make_unique<QMimeData>();
  mime_data->setUrls(QList<QUrl>({url}));

  drag->setMimeData(mime_data.release());
  auto drop_action = drag->exec(Qt::CopyAction | Qt::MoveAction);
  switch (drop_action) {
    case Qt::CopyAction:
    case Qt::MoveAction:
      HideParent();
      break;
  }
}

void SearchResultList::mousePressEvent(QMouseEvent* event) {
  switch (event->button()) {
    case Qt::LeftButton:
      starting_drag_position_ = event->pos();

      // Prevents user from deselecting an item by control-clicking it.
      if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        return;
      }

      break;
  }

  QListWidget::mousePressEvent(event);
}

void SearchResultList::AddItem(const std::shared_ptr<BaseResult>& base_result,
                               const QString& arg, int row) {
  auto key = row < kMaxCount ? QString::number(row + 1) : (const char*)0;
  auto widget = std::make_unique<SearchResult>(base_result, arg, key, this);
  auto item = std::make_unique<QListWidgetItem>(this);

  // Sets the actual height of search result items and prevents unusual sizing
  // differences between items.
  item->setSizeHint(widget->sizeHint());

  setItemWidget(item.release(), widget.release());
}

SearchResult* SearchResultList::SearchResultAt(int row) {
  return static_cast<SearchResult*>(itemWidget(item(row)));
}
