#include "searchresult.h"

#include <QFontMetrics>
#include <QIcon>
#include <Qt>

#include "../core/config.h"
#include "./ui_searchresult.h"

SearchResult::SearchResult(const std::shared_ptr<BaseResult>& base_result,
                           const QString& arg, const QString& shortcut_key,
                           QWidget* parent)
    : QWidget{parent},
      base_result_{base_result},
      parent_width_{parent->width()},
      ui_{std::make_unique<Ui::SearchResult>()} {
  ui_->setupUi(this);

  // Padding around the search result.
  ui_->horizontalLayout->setContentsMargins(kHorizontalMargin, kVerticalMargin,
                                            kHorizontalMargin, kVerticalMargin);

  // Sets height of the QLabel called `icon` to the same height of its QPixmap.
  // ui_->icon->setFixedHeight(kIconSize);
  ui_->icon->setFixedSize(Config::search_result_icon_size_,
                          Config::search_result_icon_size_);

  ui_->shortcut->setContentsMargins(0, 0, kShortcutRightMargin, 0);

  setFixedWidth(parent_width_);

  // This is required to allow QListWidget to receive mouse move events.
  setMouseTracking(true);

  SetIcon(base_result->GetIcon(), base_result->GetPixmapKey());
  SetTitle(base_result->FormatTitle(arg));
  SetDescription(base_result->GetDescription());
  SetShortcut(shortcut_key);
}

SearchResult::~SearchResult() {}

QString SearchResult::DragAndDrop() const {
  return base_result_->DragAndDrop();
}

QPixmap SearchResult::GetIcon() const { return ui_->icon->pixmap(); }

void SearchResult::HandleKeyPress(const QKeyCombination& combination,
                                  QWidget* parent) const {
  base_result_->ProcessKeyPress(combination, parent);
}

void SearchResult::HandleKeyRelease(const QKeyCombination& combination,
                                    QWidget* parent) const {
  base_result_->ProcessKeyRelease(combination, parent);
}

void SearchResult::SetDescription(const QString& description) const {
  if (description.isEmpty()) {
    ui_->description->hide();
    return;
  }

  ui_->description->setText(description);
  ui_->description->show();
}

void SearchResult::SetIcon(const QString& path,
                           const QPixmapCache::Key& key) const {
  // Tries to search for a cached QPixmap first.
  if (key.isValid()) {
    auto pixmap = QPixmap{};
    if (auto success = QPixmapCache::find(key, &pixmap); success) {
      auto result = QPixmapCache::find(key, &pixmap);
      ui_->icon->setPixmap(pixmap);
      return;
    }
  }

  // If the height of the icon is greater than the fixed height of the search
  // result, the icon height will take precedence over the fixed height, thus
  // overriding the fixed height.
  //
  // For more information regarding blurry icons:
  // https://github.com/qbittorrent/qBittorrent/issues/8841#issuecomment-387179854
  auto icon = QIcon{path};
  ui_->icon->setPixmap(icon.pixmap(Config::search_result_icon_size_));
}

void SearchResult::SetShortcut(const QString& shortcut_key) const {
  ui_->shortcut->setText(
    shortcut_key.isNull() ? shortcut_key : kShortcutModifierKey + shortcut_key);
}

void SearchResult::SetTitle(const QString& title) const {
  ui_->title->setText(title);
}

void SearchResult::resizeEvent(QResizeEvent* event) {
  auto icon_width = ui_->icon->width();
  auto title_width = ui_->title->width();
  auto shortcut_width = ui_->shortcut->width();

  // 2 for the left and right side of the horizontal layout's contents margins.
  // Another 2 for the spacing in between the horizontal layout's items.
  auto spacing = kHorizontalMargin * (2 * kHorizontalLayoutGapCount);

  auto total_width = icon_width + title_width + shortcut_width + spacing;
  if (total_width >= parent_width_) {
    auto fm = QFontMetrics{ui_->title->font()};
    auto elided_text =
      fm.elidedText(ui_->title->text(), Qt::ElideMiddle,
                    parent_width_ - icon_width - shortcut_width - spacing);
    SetTitle(elided_text);
  }
}

const int SearchResult::kHorizontalLayoutGapCount = 2;

const int SearchResult::kHorizontalMargin = 6;

const int SearchResult::kVerticalMargin = 6;

const int SearchResult::kShortcutRightMargin = 10;

const QString SearchResult::kShortcutModifierKey = "CTRL + ";
