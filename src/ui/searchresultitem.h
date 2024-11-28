#pragma once

#include <QListWidget>
#include <QListWidgetItem>
#include <cstdint>

class SearchResultItem : public QListWidgetItem {
 public:
  explicit SearchResultItem(uint64_t id, QListWidget* parent = nullptr);

  uint64_t GetId() const;

 private:
  uint64_t id_{0};
};
