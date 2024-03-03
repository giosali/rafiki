#include "searchbox.h"

#include <QObject>
#include <QtGlobal>

#include "./ui_searchbox.h"

SearchBox::SearchBox(QWidget* parent)
    : QWidget(parent), ui_(std::make_unique<Ui::SearchBox>()) {
  ui_->setupUi(this);

  // Removes spacing around the search box.
  ui_->layout->setSpacing(0);
  ui_->layout->setContentsMargins(0, 0, 0, 0);

  QObject::connect(ui_->searchBox, &QLineEdit::textChanged, this,
                   &SearchBox::SetText);
}

SearchBox::~SearchBox() {}

int SearchBox::Height() const { return ui_->searchBox->height(); }

void SearchBox::SetText(const QString& text) {
  if (text == text_) {
    return;
  }

  text_ = text;
  qDebug() << "text: " << text;
  emit TextChanged(text);
}
