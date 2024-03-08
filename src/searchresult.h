#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QString>
#include <QWidget>
#include <memory>

#include "datamodel.h"
#include "definitions.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SearchResult;
}
QT_END_NAMESPACE

class SearchResult : public QWidget {
  Q_OBJECT

 public:
  explicit SearchResult(const std::shared_ptr<DataModel>& data_model,
                        const QString& arg, const QString& shortcut_key,
                        QWidget* parent = nullptr);

  ~SearchResult();

  QString GetCommand() const;
  void PressAlt() const;
  defs::Action PressAltReturn() const;
  defs::Action PressReturn(const QString& arg) const;
  void ReleaseAlt(const QString& arg) const;
  void SetShortcut(const QString& shortcut_key) const;

 private:
  static constexpr int kFixedHeight = 44;
  static constexpr int kHorizontalMargin = 6;
  static constexpr int kVerticalMargin = 6;
  const QString kShortcutModifierKey = "CTRL + ";

  int Height() const;
  void SetDescription(const QString& description) const;
  void SetIcon(const QString& path) const;
  void SetTitle(const QString& title) const;

  std::shared_ptr<DataModel> data_model_;
  std::unique_ptr<Ui::SearchResult> ui_;
};

#endif  // SEARCHRESULT_H
