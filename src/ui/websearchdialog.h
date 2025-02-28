#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QShowEvent>
#include <Qt>
#include <cstdint>
#include <memory>

#include "interactivelabel.h"  // Necessary

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

 signals:
  void Accepted();

 private:
  void ToggleSaveButton(bool enable) const;

  QString current_icon_path_{};
  uint64_t id_{};
  QString new_icon_path_{};
  std::unique_ptr<Ui::WebSearchDialog> ui_{nullptr};

 private slots:
  void AcceptWebSearch();
  void CheckFields(const QString& text);
  void OpenFile();
};
