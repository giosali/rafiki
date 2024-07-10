#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QJsonObject>
#include <QKeyCombination>
#include <QString>
#include <cstdint>

#include "result.h"

class WebSearch : public Result {
  Q_OBJECT

 public:
  explicit WebSearch(const QJsonObject& object);
  explicit WebSearch(const QString& url, const QString& title,
                     const QString& title_placeholder, const QString& command,
                     const QString& icon, const QString& alt_url,
                     const QString& alt_title);

  bool IsCustom() const;
  QJsonObject ToJsonObject() const;

 public slots:
  void Drag() override;
  void ProcessKeyPress(const QKeyCombination& combination,
                       const Input& input) override;
  void ProcessKeyRelease(const QKeyCombination& combination,
                         const Input& input) override;

 private:
  bool ShouldAppendSpaceToCommand(const QString& title) const;

  QString alt_url_;
  bool is_custom_;
  QString url_;
};

#endif  // WEBSEARCH_H
