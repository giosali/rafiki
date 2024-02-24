#include "mainlistwidget.h"

#include <QListWidgetItem>
#include <memory>

#include "mainlistwidgetitem.h"

MainListWidget::MainListWidget(QWidget* parent) : QListWidget(parent) {
  // Component should be hidden on initialization.
  hide();
}

void MainListWidget::AddItem(const QString& text) {
  auto item = std::make_unique<QListWidgetItem>();
  auto widget = std::make_unique<MainListWidgetItem>();
  widget->SetIcon(text);

  item->setSizeHint(widget->sizeHint());
  addItem(item.get());
  setItemWidget(item.release(), widget.release());
}
