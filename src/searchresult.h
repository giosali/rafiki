#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QWidget>
#include <cstdint>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchResult;
}
QT_END_NAMESPACE

class SearchResult : public QWidget {
  Q_OBJECT

 public:
  explicit SearchResult(QWidget* parent = nullptr);

  ~SearchResult();

  constexpr int Height() const;
  void SetIcon(const QString& path) const;

 private:
  static constexpr int kFixedHeight = 44;
  static constexpr int kHorizontalMargin = 6;
  static constexpr int kVerticalMargin = 6;

  std::unique_ptr<Ui::SearchResult> ui;
};

#endif  // SEARCHRESULT_H
