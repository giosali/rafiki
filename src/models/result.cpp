#include "result.h"

#include <QFile>
#include <QFileInfo>
#include <QIcon>

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

uint64_t Result::GetId() const { return id_; }

QPixmap Result::GetPixmap() const { return pixmap_; }

QString Result::GetPixmapPath() const { return pixmap_path_; }

QString Result::GetTitle() const { return title_; }

QString Result::GetTitlePlaceholder() const { return title_placeholder_; }

bool Result::HasCommand() const { return !command_.isNull(); }

bool Result::IsEnabled() const { return is_enabled_; }

void Result::SetAltTitle(const QString& value) { alt_title_ = value; }

void Result::SetCommand(const QString& value) { command_ = value; }

void Result::SetPixmap(const QIcon& value) { pixmap_ = value.pixmap(128); }

void Result::SetPixmap(const QString& value) {
  pixmap_path_ =
    QFile::exists(value) ? value : Paths::Path(Paths::Image::kQuestionMark);
  pixmap_ = QIcon{pixmap_path_}.pixmap(128);
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
