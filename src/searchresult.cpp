#include "searchresult.h"

#include <QIcon>
#include <QLayout>
#include <QPixmap>
#include <Qt>

#include "./ui_searchresult.h"

SearchResult::SearchResult(QWidget* parent)
    : QWidget(parent), ui_(std::make_unique<Ui::SearchResult>()) {
  ui_->setupUi(this);
  ui_->horizontalLayout->setContentsMargins(kHorizontalMargin, kVerticalMargin,
                                            kHorizontalMargin, kVerticalMargin);
  setFixedHeight(kFixedHeight);
  ui_->horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
}

SearchResult::~SearchResult() {}

constexpr int SearchResult::Height() const {
  return kFixedHeight + kVerticalMargin * 2;
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
