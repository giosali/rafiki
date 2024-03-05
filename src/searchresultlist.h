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
  void AdjustSize(SearchResultList* list);
  void ProcessInput(const QString& input);
  void SetCurrentItem(SearchResultList* list);
  void UpdateShortcuts(int value);

 signals:
  void ItemsAdded(SearchResultList* list);
  void ItemsCleared(SearchResultList* list);

 private:
  static constexpr int kMaxCount = 6;

  void AddItem(const QString& icon, const QString& title,
               const QString& description, const QString& shortcut_key);

  ProjectIO project_io_;
};

#endif  // SEARCHRESULTLIST_H
