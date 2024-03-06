#include "searchresult.h"

#include <QIcon>
#include <QLayout>
#include <QPixmap>
#include <Qt>

#include "./ui_searchresult.h"
#include "searchresultlist.h"

SearchResult::SearchResult(std::shared_ptr<DataModel> data_model,
                           const QString& arg, int row, QWidget* parent)
    : QWidget(parent),
      data_model_(data_model),
      row_(row),
      ui_(std::make_unique<Ui::SearchResult>()) {
  ui_->setupUi(this);
  ui_->horizontalLayout->setContentsMargins(kHorizontalMargin, kVerticalMargin,
                                            kHorizontalMargin, kVerticalMargin);

  SetIcon(data_model_->GetIcon());
  SetTitle(data_model_->GetTitle(arg));
  SetDescription(data_model_->GetDescription());
  SetShortcut(row < SearchResultList::kMaxCount ? QString::number(row + 1)
                                                : (const char*)0);
}

SearchResult::~SearchResult() {}

int SearchResult::Height() const { return kFixedHeight + kVerticalMargin * 2; }

void SearchResult::SetShortcut(const QString& shortcut_key) const {
  ui_->shortcut->setText(shortcut_key.isNull()
                             ? shortcut_key
                             : kShortcutModifierKey + shortcut_key);
}

void SearchResult::SetShortcut(int scroll_value) const {
  if (row_ < scroll_value ||
      row_ >= scroll_value + SearchResultList::kMaxCount) {
    SetShortcut((const char*)0);
    return;
  }

  auto shortcut_key = QString::number(row_ + 1 - scroll_value);
  SetShortcut(shortcut_key);
}

void SearchResult::SetDescription(const QString& description) const {
  ui_->description->setText(description);
}

void SearchResult::SetIcon(const QString& path) const {
  // If the height of the icon is greater than the fixed height of the search
  // result, the icon height will take precedence over the fixed height, thus
  // overriding the fixed height.
  //
  // For more information regarding blurry icons:
  // https://github.com/qbittorrent/qBittorrent/issues/8841#issuecomment-387179854
  auto icon = QIcon(path);
  ui_->icon->setPixmap(icon.pixmap(kFixedHeight));
}

void SearchResult::SetTitle(const QString& title) const {
  ui_->title->setText(title);
}
