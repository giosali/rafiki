#include "mainlistwidgetitem.h"

#include "./ui_mainlistwidgetitem.h"

MainListWidgetItem::MainListWidgetItem(QWidget* parent)
    : QWidget(parent), ui(std::make_unique<Ui::MainListWidgetItem>()) {
  ui->setupUi(this);
}

MainListWidgetItem::~MainListWidgetItem() {}

void MainListWidgetItem::setIcon(const QString& text) const {
  // Setter
  ui->icon->setText(text);
}
