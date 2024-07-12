#ifndef WEBSEARCHDIALOG_H
#define WEBSEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QShowEvent>
#include <Qt>
#include <memory>
#include <vector>

#include "../core/id.h"
#include "../models/websearch.h"
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
  explicit WebSearchDialog(const Id& id, QWidget* parent = nullptr);

  ~WebSearchDialog();

  QString Test() { return "hello"; }

 public slots:
  void AcceptWebSearch();
  void CheckAltFields(const QString& text);
  void CheckMainFields(const QString& text);
  void CleanCommandField(const QString& text);
  void OpenFile();

 signals:
  void Accepted();

 protected:
  void showEvent(QShowEvent* event) override;

 private:
  std::vector<QLineEdit*> AltLineEdits() const;
  std::vector<QLineEdit*> MainLineEdits() const;
  void ToggleSaveButton(bool enable) const;

  bool close_on_show_;
  QString icon_;
  Id id_;
  std::unique_ptr<Ui::WebSearchDialog> ui_;
};

#endif  // WEBSEARCHDIALOG_H
