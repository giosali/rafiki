#include "searchresult.h"

#include <QDrag>
#include <QFontMetrics>
#include <QIcon>
#include <QMimeData>
#include <QUrl>
#include <Qt>

#include "../core/config.h"
#include "./ui_searchresult.h"

SearchResult::SearchResult(FeatureObject* object, const QString& text,
                           int index, QWidget* parent)
    : QWidget{parent},
      feature_object_{object},
      index_{index},
      parent_width_{parent->width()},
      ui_{std::make_unique<Ui::SearchResult>()} {
  ui_->setupUi(this);

  // Padding around the search result.
  ui_->horizontalLayout->setContentsMargins(kHorizontalMargin, kVerticalMargin,
                                            kHorizontalMargin, kVerticalMargin);

  // Sets height of the QLabel called `icon` to the same height of its QPixmap.
  ui_->icon->setFixedSize(Config::search_result_icon_size_,
                          Config::search_result_icon_size_);

  // Prevents Search Results from having different sizes when titles contain a
  // colon.
  ui_->title->setFixedHeight(Config::search_result_title_max_height_);

  ui_->shortcut->setContentsMargins(0, 0, kShortcutRightMargin, 0);

  setFixedWidth(parent_width_);

  // This is required to allow QListWidget to receive mouse move events.
  setMouseTracking(true);
  ui_->icon->setMouseTracking(true);
  ui_->title->setMouseTracking(true);
  ui_->description->setMouseTracking(true);
  ui_->shortcut->setMouseTracking(true);

  SetIcon(object->GetIcon());
  SetTitle(object->GetTitle());
  SetDescription(object->GetDescription());
  SetShortcut(index);

  connect(this, &SearchResult::Dragged, object, &FeatureObject::Drag);
  connect(this, &SearchResult::KeyPressed, object,
          &FeatureObject::ProcessKeyPress);
  connect(this, &SearchResult::KeyReleased, object,
          &FeatureObject::ProcessKeyRelease);
  connect(object, &FeatureObject::Dragged, this, &SearchResult::Drop);
  connect(object, &FeatureObject::NewDescriptionRequested, this,
          &SearchResult::SetDescription);
  connect(object, &FeatureObject::NewTitleRequested, this,
          &SearchResult::SetTitle);
}

SearchResult::~SearchResult() { delete feature_object_; }

void SearchResult::SetDescription(const QString& description) const {
  if (description.isEmpty()) {
    ui_->description->hide();
    return;
  }

  ui_->description->setText(description);
  ui_->description->show();
}

void SearchResult::SetIcon(const QPixmap& pixmap) const {
  // If the height of the icon is greater than the fixed height of the search
  // result, the icon height will take precedence over the fixed height, thus
  // overriding the fixed height.
  //
  // For more information regarding blurry icons:
  // https://github.com/qbittorrent/qBittorrent/issues/8841#issuecomment-387179854
  ui_->icon->setPixmap(pixmap);
}

void SearchResult::SetShortcut(int row) {
  if (row < Config::search_result_list_max_count_) {
    auto text = kShortcutModifierKey + QString::number(row + 1);
    ui_->shortcut->setText(text);

    // Stores current shortcut to revert back to when a selected item is no
    // longer selected.
    previous_shortcut_ = text;
  }
}

void SearchResult::SetShortcut(const QString& text) const {
  ui_->shortcut->setText(text);
}

void SearchResult::SetTitle(const QString& title) const {
  ui_->title->setText(title);
}

void SearchResult::Drag() {
  if (is_selected_) {
    emit Dragged();
  }
}

void SearchResult::Drop(const QString& text) {
  // Drag and drop will only apply to local files on user's machine.
  auto url = QUrl::fromLocalFile(text);

  // Takes the icon from the current search result and uses it as the icon for
  // the drop and drag action.
  auto drag = std::make_unique<QDrag>(this);
  auto icon = QIcon(ui_->icon->pixmap());
  drag->setPixmap(icon.pixmap(22));

  auto mime_data = std::make_unique<QMimeData>();
  mime_data->setUrls(QList<QUrl>({url}));

  drag->setMimeData(mime_data.release());
  drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void SearchResult::ProcessKeyPress(const QKeyCombination& combination) {
  if (is_selected_) {
    emit KeyPressed(combination);
  }
}

void SearchResult::ProcessKeyRelease(const QKeyCombination& combination) {
  if (is_selected_) {
    emit KeyReleased(combination);
  }
}

void SearchResult::SetIsSelected(int current_row) {
  if (current_row != -1) {
    is_selected_ = current_row == index_;
    SetShortcut(is_selected_ ? kReturnKey : previous_shortcut_);
  }
}

void SearchResult::UpdateShortcut(int slider_value) {
  if (index_ < slider_value) {
    return;
  }

  auto diff = index_ - slider_value;
  SetShortcut(diff);

  if (is_selected_) {
    SetShortcut(kReturnKey);
  }
}

void SearchResult::resizeEvent(QResizeEvent* event) {
  auto icon_width = ui_->icon->width();
  auto title_width = ui_->title->width();
  auto shortcut_width = ui_->shortcut->width();

  // 2 for the left and right side of the horizontal layout's contents margins.
  // Another 2 for the spacing in between the horizontal layout's items.
  auto spacing = kHorizontalMargin * (2 * kHorizontalLayoutGapCount);

  // Left-hand side is the total width.
  if (icon_width + title_width + shortcut_width + spacing >= parent_width_) {
    auto fm = QFontMetrics{ui_->title->font()};
    auto elided_text =
      fm.elidedText(ui_->title->text(), Qt::ElideMiddle,
                    parent_width_ - icon_width - shortcut_width - spacing);
    SetTitle(elided_text);
  }
}

const int SearchResult::kHorizontalLayoutGapCount = 2;

const int SearchResult::kHorizontalMargin = 6;

const QString SearchResult::kReturnKey = "↵";

const QString SearchResult::kShortcutModifierKey = "CTRL + ";

const int SearchResult::kShortcutRightMargin = 10;

const int SearchResult::kVerticalMargin = 6;
