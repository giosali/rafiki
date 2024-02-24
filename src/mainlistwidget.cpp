#include "mainlistwidget.h"

MainListWidget::MainListWidget(QWidget *parent) : QListWidget(parent) {
  // Component should be hidden on initialization.
  hide();
}
