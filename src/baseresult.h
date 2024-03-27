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

  bool CommandContainsSpace() const;
  QString FormatCommand() const;
  QString FormatTitle(const QString& arg) const;
  QString GetDescription();
  QString GetIcon();
  QUuid GetId();
  bool HasCommand() const;

 protected:
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

#endif  // BASERESULT_H
