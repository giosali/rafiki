#pragma once

#include <QWidget>
#include <Qt>
#include <cstdint>

class ToggleTab : public QWidget {
  Q_OBJECT

 public:
  ToggleTab(QWidget* parent = nullptr);

  ~ToggleTab();

 signals:
  void ModelToggled(Qt::CheckState state, uint64_t id);
};
