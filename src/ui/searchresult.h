#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QKeyCombination>
#include <QPixmap>
#include <QPixmapCache>
#include <QResizeEvent>
#include <QString>
#include <QWidget>

#include "../core/objects/featureobject.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchResult;
}
QT_END_NAMESPACE

class SearchResult : public QWidget {
  Q_OBJECT

 public:
  explicit SearchResult(FeatureObject* object, const QString& text, int index,
                        QWidget* parent = nullptr);

  ~SearchResult();

  void SetDescription(const QString& description) const;
  void SetIcon(const QPixmap& pixmap) const;
  void SetShortcut(int row);
  void SetShortcut(const QString& text) const;
  void SetTitle(const QString& title) const;

 public slots:
  void Drag();
  void Drop(const QString& text);
  void ProcessKeyPress(const QKeyCombination& combination);
  void ProcessKeyRelease(const QKeyCombination& combination);
  void SetIsSelected(int current_row);
  void UpdateShortcut(int slider_value);

 signals:
  void Dragged();
  void KeyPressed(const QKeyCombination& combination);
  void KeyReleased(const QKeyCombination& combination);

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
  static const int kHorizontalLayoutGapCount;
  static const int kHorizontalMargin;
  static const QString kReturnKey;
  static const QString kShortcutModifierKey;
  static const int kShortcutRightMargin;
  static const int kVerticalMargin;

  FeatureObject* feature_object_{nullptr};
  int index_;
  bool is_selected_;
  int parent_width_;
  QString previous_shortcut_;
  std::unique_ptr<Ui::SearchResult> ui_;
};

#endif  // SEARCHRESULT_H
