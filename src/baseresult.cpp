#include "baseresult.h"

#include <QFile>

#include "projectio.h"
#include "utils.h"

BaseResult::BaseResult(const QString &id, const QString &icon,
                       const QString &title, const QString &alt_title,
                       const QString &description, const QString &command,
                       const QString &placeholder)
    : alt_title_{alt_title},
      placeholder_{placeholder},
      command_{command},
      description_{description},
      id_{QUuid::fromString(id)} {
  SetIcon(icon);
  SetTitle(title);
}

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

void BaseResult::SetIcon(const QString &path) {
  icon_ = QFile::exists(path)
            ? path
            : ProjectIO::GetImageFilePath(ProjectIO::ImageFile::kQuestionMark);
}

void BaseResult::SetTitle(const QString &text) {
  title_ = text;
  is_title_formattable_ = text.contains(kFormat);
}
