#pragma once

#include <QIcon>
#include <QPixmap>
#include <QString>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_set>

#include "../bridges/featurebridge.h"
#include "../visitors/featurevisitable.h"

class FeatureModel : public FeatureVisitable {
 public:
  explicit FeatureModel(std::unique_ptr<FeatureBridge> bridge);

  virtual ~FeatureModel() = default;

  virtual QString FormatCommand() const;
  virtual bool ReceivesInput() const;
  virtual std::unordered_set<std::string> Tokenize() const;
  QString GetAltTitle() const;
  QString GetAltDescription() const;
  QString GetCommand() const;
  QString GetDescription() const;
  FeatureBridge* GetFeatureBridge() const;
  QPixmap GetIcon() const;
  QString GetIconPath() const;
  uint64_t GetId() const;
  bool GetIsEnabled() const;
  QString GetTitle() const;
  QString GetTitlePlaceholder() const;
  uint64_t GetUseCount() const;
  void SetIsEnabled(bool value);
  void SetUseCount(uint64_t value);

 protected:
  void SetAltDescription(const QString& value);
  void SetAltTitle(const QString& value);
  void SetCommand(const QString& value);
  void SetDescription(const QString& value);
  void SetIcon(const QIcon& value);
  void SetIcon(const QString& value);
  void SetId(uint64_t value);
  void SetTitle(const QString& value);
  void SetTitlePlaceholder(const QString& value);

 private:
  QString alt_title_{};
  QString alt_description_{};
  QString command_{};
  QString description_{};
  std::unique_ptr<FeatureBridge> feature_bridge_{nullptr};
  QPixmap icon_{};
  QString icon_path_{};
  uint64_t id_{};
  bool is_enabled_{true};
  QString title_{};
  QString title_placeholder_{};
  uint64_t use_count_{};
};
