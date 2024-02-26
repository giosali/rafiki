#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QListWidget>
#include <QString>
#include <cstdint>

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  SearchResultList(QWidget* parent = nullptr);

  int Height();

 public slots:
  void AddItem(const QString& icon_path);
  void CreateItems(const QString& text);

 signals:
  void ItemsAdded(SearchResultList* list);

 private:
  static constexpr int kMaxCount = 6;
};

#endif  // SEARCHRESULTLIST_H
