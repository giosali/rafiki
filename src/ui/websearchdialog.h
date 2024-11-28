#ifndef WEBSEARCHDIALOG_H
#define WEBSEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QShowEvent>
#include <Qt>
#include <cstdint>
#include <memory>

#include "interactivelabel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WebSearchDialog;
}
QT_END_NAMESPACE

class WebSearchDialog : public QDialog {
  Q_OBJECT

 public:
  explicit WebSearchDialog(QWidget* parent = nullptr);
  explicit WebSearchDialog(uint64_t id, QWidget* parent = nullptr);

  ~WebSearchDialog();

 public slots:
  void AcceptWebSearch();
  void CheckFields(const QString& text);
  void CleanCommandField(const QString& text);
  void OpenFile();

 signals:
  void Accepted();

 private:
  void ToggleSaveButton(bool enable) const;

  QString current_icon_path_;
  uint64_t id_{};
  QString new_icon_path_;
  std::unique_ptr<Ui::WebSearchDialog> ui_{nullptr};
};

#endif  // WEBSEARCHDIALOG_H
