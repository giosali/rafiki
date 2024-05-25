#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QKeyCombination>
#include <QPixmap>
#include <QPixmapCache>
#include <QResizeEvent>
#include <QString>
#include <QWidget>
#include <memory>

#include "../models/baseresult.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchResult;
}
QT_END_NAMESPACE

class SearchResult : public QWidget {
  Q_OBJECT

 public:
  explicit SearchResult(const std::shared_ptr<BaseResult>& base_result,
                        const QString& arg, const QString& shortcut_key,
                        int index, QWidget* parent = nullptr);

  ~SearchResult();

  QString DragAndDrop() const;
  QPixmap GetIcon() const;
  void HandleKeyPress(const QKeyCombination& combination) const;
  void HandleKeyRelease(const QKeyCombination& combination) const;
  void SetDescription(const QString& description) const;
  void SetIcon(const QString& path, const QPixmapCache::Key& key) const;
  void SetShortcut(const QString& shortcut_key) const;
  void SetTitle(const QString& title) const;

 public slots:
  void UpdateShortcut(int slider_value);

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
  static const int kHorizontalLayoutGapCount;
  static const int kHorizontalMargin;
  static const QString kShortcutModifierKey;
  static const int kShortcutRightMargin;
  static const int kVerticalMargin;

  std::shared_ptr<BaseResult> base_result_;
  int index_;
  int parent_width_;
  std::unique_ptr<Ui::SearchResult> ui_;
};

#endif  // SEARCHRESULT_H
