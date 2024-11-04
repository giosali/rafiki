#include "result.h"

#include <QFile>
#include <QFileInfo>
#include <QIcon>

#include "../core/config.h"
#include "../core/paths.h"
#include "../core/utils.h"

QString Result::FormatCommand() const {
  return append_space_to_command_ ? command_ + " " : command_;
}

QString Result::FormatTitle(const QString& arg) const {
  return is_title_formattable_
           ? utils::Format(title_, arg.isEmpty() ? title_placeholder_ : arg)
           : title_;
}

QString Result::GetAltTitle() const { return alt_title_; }

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

uint64_t Result::GetId() const { return id_; }

QString Result::GetTitle() const { return title_; }

QString Result::GetTitlePlaceholder() const { return title_placeholder_; }

bool Result::HasCommand() const { return !command_.isNull(); }

bool Result::IsEnabled() const { return is_enabled_; }

void Result::SetAltTitle(const QString& value) { alt_title_ = value; }

void Result::SetCommand(const QString& value) { command_ = value; }

void Result::SetIcon(const QString& value) {
  // Removes current pixmap key if it's set.
  if (pixmap_key_.isValid()) {
    QPixmapCache::remove(pixmap_key_);
  }

  if (!QFile::exists(value)) {
    icon_ = Paths::Path(Paths::Image::kQuestionMark);
    return;
  }

  icon_ = value;

  // Caches pixmap if too large; necessary for performance.
  if (QFileInfo{value}.size() >= 1000000) {  // 1 MB
    pixmap_key_ = QPixmapCache::insert(
      QIcon{value}.pixmap(Config::search_result_icon_size_));
  }
}

void Result::SetId(uint64_t value) { id_ = value; }

void Result::SetIsEnabled(bool value) { is_enabled_ = value; }

void Result::SetTitle(const QString& value) {
  title_ = value;
  is_title_formattable_ = value.contains("{}");
}

void Result::SetTitlePlaceholder(const QString& value) {
  title_placeholder_ = value;
}

void Result::SetAppendSpaceToCommand(bool value) {
  append_space_to_command_ = value;
}

void Result::SetDescription(const QString& value) { description_ = value; }
