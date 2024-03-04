#include "searchresult.h"

#include <QIcon>
#include <QLayout>
#include <QPixmap>
#include <Qt>

#include "./ui_searchresult.h"

SearchResult::SearchResult(const QString& icon, const QString& title,
                           const QString& description,
                           const QString& shortcut_key, QWidget* parent)
    : QWidget(parent), ui_(std::make_unique<Ui::SearchResult>()) {
  ui_->setupUi(this);
  ui_->horizontalLayout->setContentsMargins(kHorizontalMargin, kVerticalMargin,
                                            kHorizontalMargin, kVerticalMargin);
  setFixedHeight(Height());

  SetIcon(icon);
  SetTitle(title);
  SetDescription(description);
  SetShortcut(shortcut_key);
}

SearchResult::~SearchResult() {}

constexpr int SearchResult::Height() const {
  return kFixedHeight + kVerticalMargin * 2;
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

void SearchResult::SetShortcut(const QString& shortcut_key) const {
  if (shortcut_key.isNull()) {
    ui_->shortcut->hide();
    return;
  }

  auto shortcut = kShortcutModifierKey + shortcut_key;
  ui_->shortcut->setText(shortcut);
}

void SearchResult::SetTitle(const QString& title) const {
  ui_->title->setText(title);
}
