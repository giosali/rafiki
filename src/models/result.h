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
  Result();

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
  void SetId(uint64_t author_id, uint64_t id);
  void SetIsEnabled(bool value);
  void SetTitle(const QString& value);
  void SetTitlePlaceholder(const QString& value);

 protected:
  void SetAppendSpaceToCommand(bool value);
  void SetDescription(const QString& value);
  void SetIcon(const QString& value);
  void SetId(const QString& value);
  void SetPixmapKey(const QString& icon, uintmax_t icon_size);

 private:
  QString alt_title_;
  bool append_space_to_command_;
  QString command_;
  QString description_;
  QString icon_;
  Id id_;
  bool is_enabled_;
  bool is_title_formattable_;
  QPixmapCache::Key pixmap_key_;
  QString title_;
  QString title_placeholder_;
};

#endif  // RESULT_H
