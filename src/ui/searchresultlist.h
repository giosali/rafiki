#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QKeyCombination>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QObject>
#include <QPoint>
#include <QString>
#include <QThread>
#include <memory>
#include <vector>

#include "../core/input.h"
#include "../models/baseresult.h"
#include "searchresult.h"

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  static constexpr int kMaxCount = 6;

  SearchResultList(QWidget* parent = nullptr);

  ~SearchResultList();

  SearchResult* CurrentSearchResult() const;
  QString GetArg() const;
  QString GetCmd() const;
  int Height() const;
  void UpdateDescription(const QString& text);
  void UpdateTitle(const QString& text);

 public slots:
  void ActivateItem(QListWidgetItem* item);
  void AdjustSize(SearchResultList* list);
  void ProcessInput(const Input& input);
  void ProcessKeyPress(const QKeyCombination& combination);
  void ProcessKeyRelease(const QKeyCombination& combination);
  void ProcessResults(const std::vector<std::shared_ptr<BaseResult>>& results,
                      const QString& text);
  void SetCurrentItem(QListWidgetItem* item);
  void SetCurrentItem(SearchResultList* list);
  void UpdateShortcuts(int value);

 signals:
  void HideRequested();
  void InputReceived(const Input& input);
  void ItemsChanged(SearchResultList* list);
  void KeyPressSimulated(const QKeyCombination& combination);
  void TextReceived(const QString& text);

 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

 private:
  void AddItem(const std::shared_ptr<BaseResult>& base_result,
               const QString& arg, int row);
  SearchResult* SearchResultAt(int row);

  QPoint cursor_position_;
  QListWidgetItem* entered_item_;
  Input input_;
  bool is_entered_item_selectable_;
  QPoint starting_drag_position_;
  QThread worker_thread_;
};

namespace searchresultlist {
class Worker : public QObject {
  Q_OBJECT

 public slots:
  void ProcessInput(const Input& input);

 signals:
  void ResultsReadied(const std::vector<std::shared_ptr<BaseResult>>& results,
                      const QString& text);
};
}  // namespace searchresultlist

#endif  // SEARCHRESULTLIST_H
