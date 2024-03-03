#include "datamodel.h"

QString DataModel::GetDescription() { return description_; }

QString DataModel::GetIcon() { return icon_; }

QString DataModel::GetKey() { return key_; }

QString DataModel::GetTitle(const QString &input) {
  if (!is_title_formattable_) {
    return title_;
  }

  auto format_index = title_.indexOf(kFormat);
  return title_.replace(format_index, kFormat.length(), input);
}

void DataModel::SetDescription(const QString &text) { description_ = text; }

void DataModel::SetIcon(const QString &path) { icon_ = path; }

void DataModel::SetKey(const QString &text) { key_ = text; }

void DataModel::SetTitle(const QString &text) {
  title_ = text;
  is_title_formattable_ = text.contains(kFormat);
}
