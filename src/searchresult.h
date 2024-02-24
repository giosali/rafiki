#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

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
  explicit SearchResult(QWidget* parent = nullptr);

  ~SearchResult();

  void SetIcon(const QString& text) const;

 private:
  std::unique_ptr<Ui::SearchResult> ui;
};

#endif  // SEARCHRESULT_H
