#ifndef BASERESULT_H
#define BASERESULT_H

#include <QJsonObject>
#include <QString>
#include <QUuid>

#include "definitions.h"
#include "interactable.h"

class BaseResult : public Interactable {
 public:
  explicit BaseResult(const QString& id, const QString& icon,
                      const QString& title, const QString& alt_title,
                      const QString& description,
                      const QString& command = (const char*)0,
                      const QString& placeholder = (const char*)0);

  virtual ~BaseResult() = default;

  QString GetCommand(bool try_append_space) const;
  QString GetDescription();
  QString GetIcon();
  QUuid GetId();
  QString GetTitle(const QString& arg);

 protected:
  QString alt_title_;
  QString placeholder_;
  QString title_;

 private:
  const QString kFormat = "{}";

  QString command_;
  QString description_;
  bool has_command_;
  QString icon_;
  QUuid id_;
  bool is_title_formattable_;
};

#endif  // BASERESULT_H
