#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QListWidget>
#include <QString>

#include "projectio.h"

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  SearchResultList(QWidget* parent = nullptr);

  int Height() const;

 public slots:
  void ProcessInput(const QString& input);

 signals:
  void ItemsAdded(SearchResultList* list);
  void ItemsCleared(SearchResultList* list);

 private:
  static constexpr int kMaxCount = 6;

  void AddItem(const QString& icon, const QString& title,
               const QString& description);

  ProjectIO project_io_;
};

#endif  // SEARCHRESULTLIST_H
