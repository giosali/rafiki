#include "searchresult.h"

#include <QLayout>
#include <QPixmap>
#include <Qt>

#include "./ui_searchresult.h"

SearchResult::SearchResult(QWidget* parent)
    : QWidget(parent), ui(std::make_unique<Ui::SearchResult>()) {
  ui->setupUi(this);
  ui->horizontalLayout->setContentsMargins(kHorizontalMargin, kVerticalMargin,
                                           kHorizontalMargin, kVerticalMargin);
  setFixedHeight(kFixedHeight);
  ui->horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
}

SearchResult::~SearchResult() {}

constexpr int SearchResult::Height() const {
  return kFixedHeight + kVerticalMargin * 2;
}

void SearchResult::SetIcon(const QString& path) const {
  // If the height of the icon is greater than the fixed height of the search
  // result, the icon height will take precedence over the fixed height, thus
  // overriding the fixed height.
  auto pixmap = QPixmap(path);
  ui->icon->setPixmap(pixmap.scaled(kFixedHeight, kFixedHeight,
                                    Qt::IgnoreAspectRatio,
                                    Qt::SmoothTransformation));
}
