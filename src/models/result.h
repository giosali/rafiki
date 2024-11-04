#ifndef RESULT_H
#define RESULT_H

#include <QPixmap>
#include <QPixmapCache>
#include <QString>
#include <cstdint>

#include "../core/id.h"
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
  QString GetIcon() const;
  QPixmap GetIcon(int size) const;
  Id GetId() const;
  QString GetTitle() const;
  QString GetTitlePlaceholder() const;
  bool HasCommand() const;
  bool IsEnabled() const;
  void SetAltTitle(const QString& value);
  void SetCommand(const QString& value);
  void SetIcon(const QString& value);
  void SetId(uint64_t author_id, uint64_t id);
  void SetIsEnabled(bool value);
  void SetTitle(const QString& value);
  void SetTitlePlaceholder(const QString& value);

 protected:
  void SetAppendSpaceToCommand(bool value);
  void SetDescription(const QString& value);
  void SetId(const QString& value);

 private:
  QString alt_title_{};
  bool append_space_to_command_{false};
  QString command_{};
  QString description_{};
  QString icon_{};
  Id id_{0, 0};
  bool is_enabled_{true};
  bool is_title_formattable_{false};
  QPixmapCache::Key pixmap_key_{};
  QString title_{};
  QString title_placeholder_{};
};

#endif  // RESULT_H
