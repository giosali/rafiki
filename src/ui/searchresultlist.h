#ifndef SEARCHRESULTLIST_H
#define SEARCHRESULTLIST_H

#include <QKeyCombination>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QThread>
#include <memory>
#include <vector>

#include "../core/input.h"
#include "../models/baseresult.h"
#include "mainwindow.h"
#include "searchbox.h"
#include "searchresult.h"

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  SearchResultList(SearchBox* search_box, MainWindow* parent = nullptr);

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
                      const Input& input, const QString& text);
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
               const Input& input, const QString& arg, int index);
  int Height() const;
  SearchResult* SearchResultAt(int row);

  bool entered_;
  MainWindow* main_window_;
  SearchBox* search_box_;
  QPointF starting_drag_position_;
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
                      const Input& input, const QString& text);
};
}  // namespace searchresultlist

#endif  // SEARCHRESULTLIST_H
