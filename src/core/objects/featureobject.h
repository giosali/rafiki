#pragma once

#include <QIcon>
#include <QKeyCombination>
#include <QObject>
#include <QPixmap>
#include <QString>
#include <cstdint>

class FeatureModel;

class FeatureObject : public QObject {
  Q_OBJECT

 public:
  explicit FeatureObject(FeatureModel* model);
  explicit FeatureObject(FeatureModel* model, const QString& input);
  explicit FeatureObject(FeatureModel* model, const QString& input,
                         const QString& argument);

  virtual ~FeatureObject() = default;

  QString GetDescription() const;
  QPixmap GetIcon() const;
  uint64_t GetId() const;
  QString GetTitle() const;
  bool operator<(const FeatureObject& rhs) const;
  FeatureObject& operator=(const FeatureObject&) = delete;

 public slots:
  virtual void Drag() = 0;
  virtual void ProcessKeyPress(const QKeyCombination& combination);
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
  FeatureModel* GetModel() const;
  void SetDescription(const QString& value);
  void SetIcon(const QIcon& value);
  void SetId(uint64_t value);
  void SetTitle(const QString& value);

 private:
  QString description_{};
  QPixmap icon_{};
  uint64_t id_{};
  QString input_{};
  FeatureModel* model_{nullptr};
  QString title_{};
};
