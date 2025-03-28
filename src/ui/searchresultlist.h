#pragma once

#include <QKeyCombination>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QThread>
#include <QWidget>
#include <vector>

#include "../core/indexer.h"
#include "../core/objects/featureobject.h"
#include "../core/settings.h"
#include "../core/theme.h"

class SearchResultList : public QListWidget {
  Q_OBJECT

 public:
  SearchResultList(QWidget* parent = nullptr);

  ~SearchResultList();

 public slots:
  void ApplyTheme(Theme* theme);
  void ProcessKeyPress(const QKeyCombination& combination);
  void ProcessKeyRelease(const QKeyCombination& combination);
  void ProcessText(const QString& text, bool force_default_results = false);

 signals:
  void ItemDragged();
  void ItemsChanged(int height);
  void KeyPressReceived(const QKeyCombination& combination);
  void KeyReleaseReceived(const QKeyCombination& combination);
  void TextReceived(const QString& input, bool force_default_results);
  void NewSearchBoxTextRequested(const QString& text);
  void HideRequested();

 protected:
  void mouseMoveEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

 private:
  void AddItem(FeatureObject* object, const QString& text, int index);
  int Height() const;

  bool entered_{};
  QPointF starting_drag_position_{};
  bool user_selected_item_{};
  QThread worker_thread_;

 private slots:
  void ActivateItem(QListWidgetItem* item);
  void AdjustSize(int height);
  void CheckSelectedItem(QListWidgetItem* current, QListWidgetItem* previous);
  void ProcessObjects(const std::vector<FeatureObject*>& objects,
                      const QString& text, bool set_row_to_zero);
};

namespace searchresultlist {
class Worker : public QObject {
  Q_OBJECT

 public slots:
  void ProcessText(const QString& text, bool force_default_results);

 signals:
  void ObjectsReadied(const std::vector<FeatureObject*>& objects,
                      const QString& text, bool set_row_to_zero);

 private:
  Indexer& indexer_{Indexer::GetInstance()};
  Settings& settings_{Settings::GetInstance()};
};
}  // namespace searchresultlist
