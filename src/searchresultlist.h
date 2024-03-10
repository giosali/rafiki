#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QKeyCombination>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QPoint>
#include <QString>

#include "definitions.h"
#include "projectio.h"
#include "searchresult.h"

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  static constexpr int kMaxCount = 6;

  SearchResultList(QWidget* parent = nullptr);

  int Height() const;

 public slots:
  void AdjustSize(SearchResultList* list);
  void ProcessInput(const QString& input);
  void ProcessKeyPress(const QKeyCombination& key_combination);
  void ProcessKeyRelease(const QKeyCombination& key_combination);
  void SetCurrentItem(QListWidgetItem* item);
  void SetCurrentItem(SearchResultList* list);
  void UpdateShortcuts(int value);

 signals:
  void HideWindowRequested();
  void ItemsChanged(SearchResultList* list);
  void SetTextRequested(const QString& text);

 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

 private:
  void AddItem(const std::shared_ptr<DataModel>& data_model, const QString& arg,
               int row);
  void ProcessAction(defs::Action action, SearchResult* search_result);
  SearchResult* SearchResultAt(int row);

  QString arg_;
  ProjectIO project_io_;
  QPoint starting_drag_position_;
};

#endif  // SEARCHRESULTLIST_H
