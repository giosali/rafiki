#include "searchresult.h"

#include <QPixmap>
#include <Qt>

#include "./ui_searchresult.h"

SearchResult::SearchResult(QWidget* parent)
    : QWidget(parent), ui(std::make_unique<Ui::SearchResult>()) {
  ui->setupUi(this);
}

SearchResult::~SearchResult() {}

void SearchResult::SetIcon(const QString& path) const {
  auto pixmap = QPixmap(path);
  uint32_t size = 48;
  ui->icon->setPixmap(pixmap.scaled(size, size, Qt::IgnoreAspectRatio,
                                    Qt::SmoothTransformation));
}
