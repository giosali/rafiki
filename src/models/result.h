#ifndef RESULT_H
#define RESULT_H

#include <QJsonObject>
#include <QPixmap>
#include <QPixmapCache>
#include <QString>
#include <cstdint>

#include "interactable.h"

class Result : public Interactable {
  Q_OBJECT

 public:
  explicit Result(uint64_t id, const QString& icon, const QString& title,
                  const QString& title_placeholder, const QString& alt_title,
                  const QString& description, const QString& command,
                  bool append_space_to_command);

  virtual ~Result() = default;

  QString FormatCommand() const;
  QString FormatTitle(const QString& arg) const;
  QString GetCommand() const;
  QString GetDescription() const;
  QPixmap GetIcon(int size) const;
  uint64_t GetId() const;
  bool HasCommand() const;
  bool IsEnabled() const;
  void SetIsEnabled(bool value);

 protected:
  QString alt_title_;
  QString description_;
  QPixmapCache::Key pixmap_key_;
  QString title_;
  QString title_placeholder_;

 private:
  bool append_space_to_command_;
  QString command_;
  QString icon_;
  uint64_t id_;
  bool is_enabled_;
  bool is_title_formattable_;
};

#endif  // RESULT_H
