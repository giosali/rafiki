#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QString>
#include <QWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchResult;
}
QT_END_NAMESPACE

class SearchResult : public QWidget {
  Q_OBJECT

 public:
  explicit SearchResult(const QString& icon, const QString& title,
                        const QString& description = (const char*)0,
                        const QString& shortcut_key = (const char*)0,
                        QWidget* parent = nullptr);

  ~SearchResult();

  int Height() const;
  void SetDescription(const QString& description) const;
  void SetIcon(const QString& path) const;
  void SetShortcut(const QString& shortcut_key) const;
  void SetTitle(const QString& title) const;

 private:
  static constexpr int kFixedHeight = 44;
  static constexpr int kHorizontalMargin = 6;
  static constexpr int kVerticalMargin = 6;
  const QString kShortcutModifierKey = "CTRL + ";

  std::unique_ptr<Ui::SearchResult> ui_;
};

#endif  // SEARCHRESULT_H
