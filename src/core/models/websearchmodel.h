#pragma once

#include <QJsonObject>
#include <QString>

#include "featuremodel.h"

class WebSearchModel final : public FeatureModel {
 public:
  WebSearchModel(const QJsonObject& object);

  void Accept(FeatureVisitor& v) override;
  bool ReceivesInput() const override;
  QString GetAltUrl() const;
  bool GetIsCustom() const;
  QString GetUrl() const;
  void SetAltTitle(const QString& value);
  void SetAltUrl(const QString& value);
  void SetCommand(const QString& value);
  void SetIcon(const QString& value);
  void SetTitle(const QString& value);
  void SetTitlePlaceholder(const QString& value);
  void SetUrl(const QString& value);
  QJsonObject ToJson() const;

 private:
  QString alt_url_{};
  bool is_custom_{false};
  QString url_{};
};
