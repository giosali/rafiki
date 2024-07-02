#include "result.h"

#include <QFile>
#include <QIcon>

#include "../core/config.h"
#include "../core/io.h"
#include "../core/utils.h"

Result::Result(uint64_t id)
    : alt_title_{QString{}},
      append_space_to_command_{false},
      command_{QString{}},
      description_{QString{}},
      icon_{QString{}},
      id_{id},
      is_enabled_{true},
      is_title_formattable_{false},
      pixmap_key_{QPixmapCache::Key{}},
      title_{QString{}},
      title_placeholder_{QString{}} {}

QString Result::Command() const { return command_; }

QString Result::Description() const { return description_; }

QString Result::FormatCommand() const {
  return append_space_to_command_ ? command_ + " " : command_;
}

QString Result::FormatTitle(const QString &arg) const {
  return is_title_formattable_
           ? utils::Format(title_, arg.isEmpty() ? title_placeholder_ : arg)
           : title_;
}

bool Result::HasCommand() const { return !command_.isNull(); }

QPixmap Result::Icon(int size) const {
  // Tries to search for a cached QPixmap first.
  if (pixmap_key_.isValid()) {
    if (auto pixmap = QPixmap{}; QPixmapCache::find(pixmap_key_, &pixmap)) {
      return pixmap;
    }
  }

  return QIcon{icon_}.pixmap(size);
}

uint64_t Result::Id() const { return id_; }

bool Result::IsEnabled() const { return is_enabled_; }

void Result::SetIsEnabled(bool value) { is_enabled_ = value; }

void Result::SetAltTitle(const QString &value) { alt_title_ = value; }

void Result::SetAppendSpaceToCommand(bool value) {
  append_space_to_command_ = value;
}

void Result::SetCommand(const QString &value) { command_ = value; }

void Result::SetDescription(const QString &value) { description_ = value; }

void Result::SetIcon(const QString &value) {
  icon_ = QFile::exists(value) ? value
                               : Io::GetFilePath(Io::ImageFile::kQuestionMark);
}

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

QString Result::Title() const { return title_; }

QString Result::TitlePlaceholder() const { return title_placeholder_; }
