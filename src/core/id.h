#ifndef ID_H
#define ID_H

#include <QString>
#include <QVariant>
#include <cstdint>
#include <cstdlib>

class Id {
 public:
  Id();
  explicit Id(uint64_t author_id, uint64_t result_id);
  explicit Id(const QString& tuple);
  explicit Id(const QVariant& tuple);

  QString Escape() const;
  uint64_t GetAuthorId() const;
  uint64_t GetResultId() const;
  bool IsNull() const;
  QString ToString() const;
  bool operator==(const Id& id) const;

 private:
  uint64_t author_id_;
  uint64_t result_id_;
};

template <>
struct std::hash<Id> {
  size_t operator()(const Id& id) const {
    auto a = id.GetAuthorId();
    auto b = id.GetResultId();

    // https://stackoverflow.com/a/13871379
    return a >= b ? a * a + a + b : a + b * b;
  }
};

#endif  // ID_H
