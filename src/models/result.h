#ifndef RESULT_H
#define RESULT_H

#include <QIcon>
#include <QPixmap>
#include <QString>
#include <cstdint>

#include "interactable.h"

class Result : public Interactable {
  Q_OBJECT

 public:
  Result() = default;

  virtual ~Result() = default;

  QString FormatCommand() const;
  QString FormatTitle(const QString& arg) const;
  QString GetAltTitle() const;
  QString GetCommand() const;
  QString GetDescription() const;
  uint64_t GetId() const;
  QPixmap GetPixmap() const;
  QString GetPixmapPath() const;
  QString GetTitle() const;
  QString GetTitlePlaceholder() const;
  bool HasCommand() const;
  bool IsEnabled() const;
  void SetAltTitle(const QString& value);
  void SetCommand(const QString& value);
  void SetPixmap(const QIcon& value);
  void SetPixmap(const QString& value);
  void SetId(uint64_t value);
  void SetIsEnabled(bool value);
  void SetTitle(const QString& value);
  void SetTitlePlaceholder(const QString& value);

 protected:
  void SetAppendSpaceToCommand(bool value);
  void SetDescription(const QString& value);

 private:
  QString alt_title_{};
  bool append_space_to_command_{false};
  QString command_{};
  QString description_{};
  uint64_t id_{0};
  bool is_enabled_{true};
  bool is_title_formattable_{false};
  QPixmap pixmap_{};
  QString pixmap_path_{};
  QString title_{};
  QString title_placeholder_{};
};

#endif  // RESULT_H
