#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QJsonObject>
#include <QString>
#include <QUuid>

#include "definitions.h"

class DataModel {
 public:
  explicit DataModel() = default;

  virtual ~DataModel() = default;

  virtual defs::Action AltGo(const QString& arg) = 0;
  virtual QString GetAltTitle() = 0;
  virtual defs::Action Go(const QString& arg) = 0;
  virtual void Populate(const QJsonObject& object) = 0;
  QString GetCommand(bool try_append_space);
  QString GetDescription();
  QString GetIcon();
  QUuid GetId();
  QString GetTitle(const QString& arg);

 protected:
  void SetCommand(const QString& text);
  void SetDescription(const QString& text);
  void SetIcon(const QString& path);
  void SetId(const QString& id);
  void SetPlaceholder(const QString& text);
  void SetTitle(const QString& text);

 private:
  const QString kFormat = "{}";

  QString command_;
  QString description_;
  QString icon_;
  QUuid id_;
  bool is_title_formattable_;
  QString placeholder_;
  QString title_;
};

#endif  // DATAMODEL_H
