#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QEvent>
#include <QKeyCombination>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QPoint>
#include <QString>
#include <memory>

#include "baseresult.h"
#include "searchresult.h"

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  static constexpr int kMaxCount = 6;

  SearchResultList(QWidget* parent = nullptr);

  SearchResult* CurrentSearchResult() const;
  QString GetArg() const;
  QString GetCmd() const;
  int Height() const;
  void HideParent();

 public slots:
  void ActivateItem(QListWidgetItem* item);
  void AdjustSize(SearchResultList* list);
  void ProcessInput(const QString& input);
  void ProcessKeyPress(const QKeyCombination& combination);
  void ProcessKeyRelease(const QKeyCombination& combination);
  void SetCurrentItem(QListWidgetItem* item);
  void SetCurrentItem(SearchResultList* list);
  void UpdateShortcuts(int value);

 signals:
  void EventReceived(QEvent* event);
  void ItemsChanged(SearchResultList* list);
  void TextReceived(const QString& text);

 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

 private:
  void AddItem(const std::shared_ptr<BaseResult>& base_result,
               const QString& arg, int row);
  SearchResult* SearchResultAt(int row);

  QString arg_;
  QString cmd_;
  QPoint starting_drag_position_;
};

#endif  // SEARCHRESULTLIST_H
