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
                        QWidget* parent = nullptr);

  ~SearchResult();

  QString DragAndDrop() const;
  QPixmap GetIcon() const;
  void HandleKeyPress(const QKeyCombination& combination,
                      QWidget* parent) const;
  void HandleKeyRelease(const QKeyCombination& combination,
                        QWidget* parent) const;
  void SetDescription(const QString& description) const;
  void SetIcon(const QString& path, const QPixmapCache::Key& key) const;
  void SetShortcut(const QString& shortcut_key) const;
  void SetTitle(const QString& title) const;

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
  static constexpr int kHorizontalLayoutGapCount = 2;
  static constexpr int kHorizontalMargin = 6;
  static constexpr int kVerticalMargin = 6;
  static constexpr int kShortcutRightMargin = 10;
  const QString kShortcutModifierKey = "CTRL + ";

  std::shared_ptr<BaseResult> base_result_;
  int parent_width_;
  std::unique_ptr<Ui::SearchResult> ui_;
};

#endif  // SEARCHRESULT_H
