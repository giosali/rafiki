#include "id.h"

#include <QStringList>

Id::Id() : author_id_{0}, result_id_{0} {}

Id::Id(uint64_t author_id, uint64_t result_id)
    : author_id_{author_id}, result_id_{result_id} {}

Id::Id(const QString& tuple) {
  auto parts = tuple.split(':');
  author_id_ = parts[0].toULongLong();
  result_id_ = parts[1].toULongLong();
}

Id::Id(const QVariant& tuple) : Id{tuple.toString()} {}

uint64_t Id::GetAuthorId() const { return author_id_; }

uint64_t Id::GetResultId() const { return result_id_; }

QString Id::ToString() const {
  return QString{"%1:%2"}.arg(author_id_, result_id_);
}

bool Id::operator==(const Id& id) const {
  return author_id_ == id.author_id_ && result_id_ == id.result_id_;
}
