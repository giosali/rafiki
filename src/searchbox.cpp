#include "searchbox.h"

#include <QObject>
#include <QtGlobal>

#include "./ui_searchbox.h"

SearchBox::SearchBox(QWidget* parent)
    : QWidget(parent), ui(std::make_unique<Ui::SearchBox>()) {
  ui->setupUi(this);

  // Removes spacing around the search box.
  ui->layout->setSpacing(0);
  ui->layout->setContentsMargins(0, 0, 0, 0);

  QObject::connect(ui->searchBox, &QLineEdit::textChanged, this,
                   &SearchBox::SetText);
}

SearchBox::~SearchBox() {}

void SearchBox::SetText(const QString& text) {
  if (text == text_) {
    return;
  }

  text_ = text;
  qDebug() << "text: " << text;
  emit TextChanged(text);
}
