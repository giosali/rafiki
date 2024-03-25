#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QJsonObject>
#include <QString>
#include <QUuid>

#include "definitions.h"
#include "interactable.h"

class DataModel : public Interactable {
 public:
  DataModel() = default;
  explicit DataModel(const QString& id, const QString& icon,
                     const QString& title, const QString& alt_title,
                     const QString& description, const QString& command,
                     const QString& placeholder = (const char*)0);

  virtual ~DataModel() = default;

  QString GetCommand(bool try_append_space) const;
  QString GetDescription();
  QString GetIcon();
  QUuid GetId();
  QString GetTitle(const QString& arg);

 protected:
  void SetIcon(const QString& path);
  void SetTitle(const QString& text);

  QString alt_title_;
  QString placeholder_;
  QString title_;

 private:
  const QString kFormat = "{}";

  QString command_;
  QString description_;
  QString icon_;
  QUuid id_;
  bool is_title_formattable_;
};

#endif  // DATAMODEL_H
