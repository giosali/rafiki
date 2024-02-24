#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QListWidget>

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  SearchResultList(QWidget* parent = nullptr);

  void AddItem(const QString& text);
};

#endif  // SEARCHRESULTLIST_H
