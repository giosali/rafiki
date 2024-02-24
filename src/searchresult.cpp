#include "searchresult.h"

#include "./ui_searchresult.h"

SearchResult::SearchResult(QWidget* parent)
    : QWidget(parent), ui(std::make_unique<Ui::SearchResult>()) {
  ui->setupUi(this);
}

SearchResult::~SearchResult() {}

void SearchResult::SetIcon(const QString& text) const {
  // Setter
  ui->icon->setText(text);
}
