#include "datamodel.h"

#include <QFile>

#include "projectio.h"
#include "utils.h"

DataModel::DataModel(const QString &id, const QString &icon,
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

QString DataModel::GetCommand(bool try_append_space) const {
  return try_append_space && is_title_formattable_ ? command_ + " " : command_;
}

QString DataModel::GetDescription() { return description_; }

QString DataModel::GetIcon() { return icon_; }

QUuid DataModel::GetId() { return id_; }

QString DataModel::GetTitle(const QString &arg) {
  return is_title_formattable_
           ? utils::Format(title_, arg.isEmpty() ? placeholder_ : arg)
           : title_;
}

void DataModel::SetIcon(const QString &path) {
  icon_ = QFile::exists(path)
            ? path
            : ProjectIO::GetImageFilePath(ProjectIO::ImageFile::kQuestionMark);
}

void DataModel::SetTitle(const QString &text) {
  title_ = text;
  is_title_formattable_ = text.contains(kFormat);
}
