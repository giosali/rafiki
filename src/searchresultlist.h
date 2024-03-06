#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QListWidget>
#include <QString>

#include "projectio.h"

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  static constexpr int kMaxCount = 6;

  SearchResultList(QWidget* parent = nullptr);

  int Height() const;

 public slots:
  void AdjustSize(SearchResultList* list);
  void ProcessInput(const QString& input);
  void ProcessKeyPress(int key);
  void SetCurrentItem(SearchResultList* list);

 signals:
  void ItemsAdded(SearchResultList* list);
  void ItemsCleared(SearchResultList* list);

 private:
  void AddItem(std::shared_ptr<DataModel> data_model, const QString& arg,
               int row);

  ProjectIO project_io_;
};

#endif  // SEARCHRESULTLIST_H
