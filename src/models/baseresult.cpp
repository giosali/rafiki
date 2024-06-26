#include "baseresult.h"

#include <QFile>
#include <QIcon>

#include "../core/io.h"
#include "../core/utils.h"

BaseResult::BaseResult(uint64_t id, const QString &icon, const QString &title,
                       const QString &title_placeholder,
                       const QString &alt_title, const QString &description,
                       const QString &command, bool append_space_to_command)
    : alt_title_{alt_title},
      append_space_to_command_{append_space_to_command},
      command_{command},
      description_{description},
      icon_{QFile::exists(icon) ? icon
                                : Io::GetIcon(Io::ImageFile::kQuestionMark)},
      id_{id},
      is_enabled_{true},
      is_title_formattable_{title.contains("{}")},
      title_placeholder_{title_placeholder},
      title_{title} {}

QString BaseResult::FormatCommand() const {
  return append_space_to_command_ ? command_ + " " : command_;
}

QString BaseResult::FormatTitle(const QString &arg) const {
  return is_title_formattable_
           ? utils::Format(title_, arg.isEmpty() ? title_placeholder_ : arg)
           : title_;
}

QString BaseResult::GetCommand() const { return command_; }

QString BaseResult::GetDescription() const { return description_; }

QPixmap BaseResult::GetIcon(int size) const {
  // Tries to search for a cached QPixmap first.
  if (pixmap_key_.isValid()) {
    if (auto pixmap = QPixmap{}; QPixmapCache::find(pixmap_key_, &pixmap)) {
      return pixmap;
    }
  }

  return QIcon{icon_}.pixmap(size);
}

uint64_t BaseResult::GetId() const { return id_; }

bool BaseResult::HasCommand() const { return !command_.isNull(); }

bool BaseResult::IsEnabled() const { return is_enabled_; }
