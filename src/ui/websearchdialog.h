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
  explicit WebSearchDialog(uint64_t id, QWidget* parent = nullptr);

  ~WebSearchDialog();

 signals:
  void Accepted();

 private:
  const static QString kFallbackIcon;

  bool AreAltFieldsValid(const QString& alt_url,
                         const QString& alt_title) const;
  bool AreRequiredFieldsValid(const QString& url, const QString& title,
                              const QString& title_placeholder,
                              const QString& command) const;
  QString SaveIcon(const QString& path) const;

  QString icon_path_{};
  uint64_t id_{};
  bool is_new_{true};
  std::unique_ptr<Ui::WebSearchDialog> ui_{nullptr};

 private slots:
  void AcceptWebSearch();
  void CheckFields(const QString& text);
  void OpenFile();
};
