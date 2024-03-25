#include "searchresult.h"

#include <QIcon>
#include <QPixmap>
#include <Qt>

#include "./ui_searchresult.h"

SearchResult::SearchResult(const std::shared_ptr<BaseResult>& base_result,
                           const QString& arg, const QString& shortcut_key,
                           QWidget* parent)
    : QWidget{parent},
      base_result_{base_result},
      ui_{std::make_unique<Ui::SearchResult>()} {
  ui_->setupUi(this);
  ui_->horizontalLayout->setContentsMargins(kHorizontalMargin, kVerticalMargin,
                                            kHorizontalMargin, kVerticalMargin);
  setFixedHeight(Height());

  // This is required to allow QListWidget to receive mouse move events.
  setMouseTracking(true);

  SetIcon(base_result->GetIcon());
  SetTitle(base_result->GetTitle(arg));
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

int SearchResult::Height() const { return kFixedHeight + kVerticalMargin * 2; }

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
  auto icon = QIcon{path};
  ui_->icon->setPixmap(icon.pixmap(kFixedHeight));
}

void SearchResult::SetShortcut(const QString& shortcut_key) const {
  ui_->shortcut->setText(
    shortcut_key.isNull() ? shortcut_key : kShortcutModifierKey + shortcut_key);
}

void SearchResult::SetTitle(const QString& title) const {
  ui_->title->setText(title);
}
