#ifndef BASERESULT_H
#define BASERESULT_H

#include <QJsonObject>
#include <QPixmap>
#include <QPixmapCache>
#include <QString>
#include <QUuid>

#include "interactable.h"

class BaseResult : public Interactable {
  Q_OBJECT

 public:
  explicit BaseResult(const QString& id, const QString& icon,
                      const QString& title, const QString& title_placeholder,
                      const QString& alt_title, const QString& description,
                      const QString& command, bool append_space_to_command);

  virtual ~BaseResult() = default;

  QString FormatCommand() const;
  QString FormatTitle(const QString& arg) const;
  QString GetDescription();
  QPixmap GetIcon(int size);
  QUuid GetId();
  bool HasCommand() const;

 protected:
  QString alt_title_;
  QString description_;
  QPixmapCache::Key pixmap_key_;
  QString title_placeholder_;
  QString title_;

 private:
  const QString kFormat = "{}";

  bool append_space_to_command_;
  QString command_;
  QString icon_;
  QUuid id_;
  bool is_title_formattable_;
};

#endif  // BASERESULT_H
