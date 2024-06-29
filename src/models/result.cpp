#include "result.h"

#include <QFile>
#include <QIcon>

#include "../core/io.h"
#include "../core/utils.h"

Result::Result(uint64_t id, const QString &icon, const QString &title,
               const QString &title_placeholder, const QString &alt_title,
               const QString &description, const QString &command,
               bool append_space_to_command)
    : alt_title_{alt_title},
      append_space_to_command_{append_space_to_command},
      command_{command},
      description_{description},
      icon_{QFile::exists(icon)
              ? icon
              : Io::GetFilePath(Io::ImageFile::kQuestionMark)},
      id_{id},
      is_enabled_{true},
      is_title_formattable_{title.contains("{}")},
      title_placeholder_{title_placeholder},
      title_{title} {}

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

QPixmap Result::GetIcon(int size) const {
  // Tries to search for a cached QPixmap first.
  if (pixmap_key_.isValid()) {
    if (auto pixmap = QPixmap{}; QPixmapCache::find(pixmap_key_, &pixmap)) {
      return pixmap;
    }
  }

  return QIcon{icon_}.pixmap(size);
}

uint64_t Result::GetId() const { return id_; }

bool Result::HasCommand() const { return !command_.isNull(); }

bool Result::IsEnabled() const { return is_enabled_; }

void Result::SetIsEnabled(bool value) { is_enabled_ = value; }
