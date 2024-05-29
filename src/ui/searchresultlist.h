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
  SearchResultList(QWidget* parent = nullptr);

  ~SearchResultList();

  void UpdateDescription(const QString& text);
  void UpdateTitle(const QString& text);

 public slots:
  void ActivateItem(QListWidgetItem* item);
  void AdjustSize(int height);
  void CheckSelectedItem(QListWidgetItem* current, QListWidgetItem* previous);
  void ProcessInput(const Input& input);
  void ProcessKeyPress(const QKeyCombination& combination);
  void ProcessKeyRelease(const QKeyCombination& combination);
  void ProcessResults(const std::vector<std::shared_ptr<BaseResult>>& results,
                      const QString& text);
  void SetUserSelectedItem(bool value);

 signals:
  void ItemDragged();
  void HideRequested();
  void InputReceived(const Input& input);
  void ItemsChanged(int height);
  void KeyPressReceived(const QKeyCombination& combination);
  void KeyReleaseReceived(const QKeyCombination& combination);

 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

 private:
  void AddItem(const std::shared_ptr<BaseResult>& base_result,
               const QString& arg, int index);
  int Height() const;
  SearchResult* SearchResultAt(int row);

  QPoint cursor_position_;
  bool entered_;
  QPoint starting_drag_position_;
  bool user_selected_item_;
  QThread worker_thread_;
};

namespace searchresultlist {
class Worker : public QObject {
  Q_OBJECT

 public slots:
  void ProcessInput(const Input& input);

 signals:
  void DefaultResultsGuardChanged(bool value);
  void ResultsReadied(const std::vector<std::shared_ptr<BaseResult>>& results,
                      const QString& text);
};
}  // namespace searchresultlist

#endif  // SEARCHRESULTLIST_H
