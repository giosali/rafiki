#include "baseresult.h"

#include <QFile>

#include "../core/definitions.h"
#include "../core/project.h"
#include "../core/utils.h"

BaseResult::BaseResult(const QString &id, const QString &icon,
                       const QString &title, const QString &title_placeholder,
                       const QString &alt_title, const QString &description,
                       const QString &command, bool append_space_to_command)
    : alt_title_{alt_title},
      command_{command},
      description_{description},
      icon_{QFile::exists(icon)
              ? icon
              : Project::GetImageFilePath(defs::ImageFile::kQuestionMark)},
      id_{QUuid::fromString(id)},
      is_title_formattable_{title.contains(kFormat)},
      title_placeholder_{title_placeholder},
      title_{title} {}

bool BaseResult::CommandContainsSpace() const { return command_.contains(" "); }

QString BaseResult::FormatCommand() const {
  return is_title_formattable_ ? command_ + " " : command_;
}

QString BaseResult::FormatTitle(const QString &arg) const {
  return is_title_formattable_
           ? utils::Format(title_, arg.isEmpty() ? title_placeholder_ : arg)
           : title_;
}

QString BaseResult::GetDescription() { return description_; }

QString BaseResult::GetIcon() { return icon_; }

QUuid BaseResult::GetId() { return id_; }

bool BaseResult::HasCommand() const { return !command_.isNull(); }
