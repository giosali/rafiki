#pragma once

#include <QKeyCombination>
#include <QObject>
#include <QPixmap>
#include <QString>
#include <cstdint>

class FeatureModel;

class FeatureObject : public QObject {
  Q_OBJECT

 public:
  explicit FeatureObject(const FeatureModel* model);
  explicit FeatureObject(const FeatureModel* model, const QString& input);

  virtual ~FeatureObject() = default;

  QString GetDescription() const;
  QPixmap GetIcon() const;
  uint64_t GetId() const;
  QString GetTitle() const;
  FeatureObject& operator=(const FeatureObject&) = delete;

 public slots:
  virtual void Drag() = 0;
  virtual void ProcessKeyPress(const QKeyCombination& combination) = 0;
  virtual void ProcessKeyRelease(const QKeyCombination& combination) = 0;

 signals:
  void Dragged(const QString& text);
  void Hidden();
  void NewDescriptionRequested(const QString& text);
  void NewSearchBoxTextRequested(const QString& text);
  void NewTitleRequested(const QString& text);

 protected:
  QString GetAltDescription() const;
  QString GetAltTitle() const;
  void SetDescription(const QString& value);
  void SetIcon(const QPixmap& value);
  void SetId(uint64_t value);
  void SetTitle(const QString& value);

 private:
  QString alt_description_{};
  QString alt_title_{};
  QString description_{};
  QPixmap icon_{};
  uint64_t id_{};
  QString title_{};
};
