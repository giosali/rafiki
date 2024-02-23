#include "mainlistview.h"

MainListView::MainListView(QWidget *parent) : QListView(parent) {
  // Component should be hidden on initialization.
  hide();
}
