#pragma once

#include <QObject>
#include <QTest>

class TestHtrieMap : public QObject {
  Q_OBJECT

 private slots:
  void EqualPrefixRange_CommonPrefix();
  void EqualPrefixRange_EraseAfterInsert();
  void EqualPrefixRange_EraseSharedKey();
  void Find_EraseSharedKeyAndKeepRemainder();
};
