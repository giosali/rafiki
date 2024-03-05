#include "datamodel.h"

#include <QFile>

#include "projectio.h"
#include "utils.h"

QString DataModel::GetDescription() { return description_; }

QString DataModel::GetIcon() { return icon_; }

QString DataModel::GetCommand(bool try_append_space) {
  return try_append_space && is_title_formattable_ ? command_ + " " : command_;
}

QString DataModel::GetTitle(const QString &arg) {
  return is_title_formattable_
             ? utils::Format(title_, arg.isEmpty() ? placeholder_ : arg)
             : title_;
}

void DataModel::SetCommand(const QString &text) { command_ = text; }

void DataModel::SetDescription(const QString &text) { description_ = text; }

void DataModel::SetIcon(const QString &path) {
  icon_ =
      QFile::exists(path)
          ? path
          : ProjectIO::GetImageFilePath(ProjectIO::ImageFile::kQuestionMark);
}

void DataModel::SetPlaceholder(const QString &text) { placeholder_ = text; }

void DataModel::SetTitle(const QString &text) {
  title_ = text;
  is_title_formattable_ = text.contains(kFormat);
}
