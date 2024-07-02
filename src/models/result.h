#ifndef RESULT_H
#define RESULT_H

#include <QPixmap>
#include <QPixmapCache>
#include <QString>
#include <cstdint>

#include "interactable.h"

class Result : public Interactable {
  Q_OBJECT

 public:
  explicit Result(uint64_t id);

  virtual ~Result() = default;

  QString Command() const;
  QString Description() const;
  QString FormatCommand() const;
  QString FormatTitle(const QString& arg) const;
  bool HasCommand() const;
  QPixmap Icon(int size) const;
  uint64_t Id() const;
  bool IsEnabled() const;
  void SetIsEnabled(bool value);

 protected:
  void SetAltTitle(const QString& value);
  void SetAppendSpaceToCommand(bool value);
  void SetCommand(const QString& value);
  void SetDescription(const QString& value);
  void SetIcon(const QString& value);
  void SetPixmapKey(const QString& icon, uintmax_t icon_size);
  void SetTitle(const QString& value);
  void SetTitlePlaceholder(const QString& value);
  QString Title() const;
  QString TitlePlaceholder() const;

 private:
  QString alt_title_;
  bool append_space_to_command_;
  QString command_;
  QString description_;
  QString icon_;
  uint64_t id_;
  bool is_enabled_;
  bool is_title_formattable_;
  QPixmapCache::Key pixmap_key_;
  QString title_;
  QString title_placeholder_;
};

#endif  // RESULT_H
