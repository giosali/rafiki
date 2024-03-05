#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QJsonObject>
#include <QString>

class DataModel {
 public:
  explicit DataModel() = default;

  virtual ~DataModel() = default;

  virtual void Populate(const QJsonObject& object) = 0;
  QString GetCommand(bool try_append_space);
  QString GetDescription();
  QString GetIcon();
  QString GetTitle(const QString& arg);

 protected:
  void SetCommand(const QString& text);
  void SetDescription(const QString& text);
  void SetIcon(const QString& path);
  void SetPlaceholder(const QString& text);
  void SetTitle(const QString& text);

 private:
  const QString kFormat = "{}";

  QString command_;
  QString description_;
  QString icon_;
  bool is_title_formattable_;
  QString placeholder_;
  QString title_;
};

#endif  // DATAMODEL_H
