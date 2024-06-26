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
#include "../models/result.h"
#include "mainwindow.h"
#include "searchbox.h"
#include "searchresult.h"

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  SearchResultList(SearchBox* search_box, MainWindow* parent = nullptr);

  ~SearchResultList();

 public slots:
  void ActivateItem(QListWidgetItem* item);
  void AdjustSize(int height);
  void CheckSelectedItem(QListWidgetItem* current, QListWidgetItem* previous);
  void ProcessInput(const Input& input);
  void ProcessKeyPress(const QKeyCombination& combination);
  void ProcessKeyRelease(const QKeyCombination& combination);
  void ProcessResults(const std::vector<std::shared_ptr<Result>>& results,
                      const Input& input, const QString& argument,
                      bool set_row_to_zero);
  void SetUserSelectedItem(bool value);

 signals:
  void ItemDragged();
  void InputReceived(const Input& input);
  void ItemsChanged(int height);
  void KeyPressReceived(const QKeyCombination& combination);
  void KeyReleaseReceived(const QKeyCombination& combination);

 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

 private:
  void AddItem(const std::shared_ptr<Result>& result, const Input& input,
               const QString& argument, int index);
  int Height() const;

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
  void ResultsReadied(const std::vector<std::shared_ptr<Result>>& results,
                      const Input& input, const QString& text,
                      bool set_row_to_zero);
};
}  // namespace searchresultlist

#endif  // SEARCHRESULTLIST_H
