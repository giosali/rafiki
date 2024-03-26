#include "baseresult.h"

#include <QFile>

#include "definitions.h"
#include "project.h"
#include "utils.h"

BaseResult::BaseResult(const QString &id, const QString &icon,
                       const QString &title, const QString &alt_title,
                       const QString &description, const QString &command,
                       const QString &placeholder)
    : alt_title_{alt_title},
      placeholder_{placeholder},
      title_{title},
      command_{command},
      description_{description},
      has_command_{!command.isNull()},
      icon_{QFile::exists(icon)
              ? icon
              : Project::GetImageFilePath(defs::ImageFile::kQuestionMark)},
      id_{QUuid::fromString(id)},
      is_title_formattable_{title.contains(kFormat)} {}

QString BaseResult::GetCommand(bool try_append_space) const {
  return try_append_space && is_title_formattable_ ? command_ + " " : command_;
}

QString BaseResult::GetDescription() { return description_; }

QString BaseResult::GetIcon() { return icon_; }

QUuid BaseResult::GetId() { return id_; }

QString BaseResult::GetTitle(const QString &arg) {
  return is_title_formattable_
           ? utils::Format(title_, arg.isEmpty() ? placeholder_ : arg)
           : title_;
}
