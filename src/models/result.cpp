#include "result.h"

#include <QFile>
#include <QIcon>

#include "../core/config.h"
#include "../core/io.h"
#include "../core/utils.h"

Result::Result()
    : alt_title_{},
      alt_title_placeholder_{},
      append_space_to_command_{false},
      command_{},
      description_{},
      icon_{},
      id_{0, 0},
      is_alt_title_formattable_{false},
      is_enabled_{true},
      is_title_formattable_{false},
      pixmap_key_{},
      title_{},
      title_placeholder_{} {}

QString Result::FormatAltTitle(const QString &arg) const {
  return is_alt_title_formattable_
           ? utils::Format(alt_title_,
                           arg.isEmpty() ? alt_title_placeholder_ : arg)
           : alt_title_;
}

QString Result::FormatCommand() const {
  return append_space_to_command_ ? command_ + " " : command_;
}

QString Result::FormatTitle(const QString &arg) const {
  return is_title_formattable_
           ? utils::Format(title_, arg.isEmpty() ? title_placeholder_ : arg)
           : title_;
}

QString Result::GetCommand() const { return command_; }

QString Result::GetDescription() const { return description_; }

QString Result::GetIcon() const { return icon_; }

QPixmap Result::GetIcon(int size) const {
  // Tries to search for a cached QPixmap first.
  if (pixmap_key_.isValid()) {
    if (auto pixmap = QPixmap{}; QPixmapCache::find(pixmap_key_, &pixmap)) {
      return pixmap;
    }
  }

  return QIcon{icon_}.pixmap(size);
}

Id Result::GetId() const { return id_; }

bool Result::HasCommand() const { return !command_.isNull(); }

bool Result::IsEnabled() const { return is_enabled_; }

void Result::SetId(uint64_t author_id, uint64_t id) { id_ = Id{author_id, id}; }

void Result::SetIsEnabled(bool value) { is_enabled_ = value; }

QString Result::GetAltTitle() const { return alt_title_; }

QString Result::GetAltTitlePlaceholder() const {
  return alt_title_placeholder_;
}

QString Result::GetTitle() const { return title_; }

QString Result::GetTitlePlaceholder() const { return title_placeholder_; }

void Result::SetAltTitle(const QString &value) { alt_title_ = value; }

void Result::SetAltTitlePlaceholder(const QString &value) {
  alt_title_placeholder_ = value;
  is_alt_title_formattable_ = value.contains("{}");
}

void Result::SetAppendSpaceToCommand(bool value) {
  append_space_to_command_ = value;
}

void Result::SetCommand(const QString &value) { command_ = value; }

void Result::SetDescription(const QString &value) { description_ = value; }

void Result::SetIcon(const QString &value) {
  icon_ =
    QFile::exists(value) ? value : Io::GetFilePath(Io::Image::kQuestionMark);
}

void Result::SetId(const QString &value) { id_ = Id{value}; }

void Result::SetPixmapKey(const QString &icon, uintmax_t icon_size) {
  if (icon_size >= 1000000) {  // 1 MB
    pixmap_key_ = QPixmapCache::insert(
      QIcon{icon}.pixmap(Config::search_result_icon_size_));
  }
}

void Result::SetTitle(const QString &value) {
  title_ = value;
  is_title_formattable_ = value.contains("{}");
}

void Result::SetTitlePlaceholder(const QString &value) {
  title_placeholder_ = value;
}
