#include "utilities_test.h"

#include <QChar>
#include <QList>
#include <QString>
#include <QTest>

#include "core/utilities.h"

void TestUtilities::Combine() {
  QFETCH(QString, p1);
  QFETCH(QString, p2);
  QFETCH(QString, expected);

  auto actual = Utilities::Combine(p1, p2);
  QCOMPARE(actual, expected);
}

void TestUtilities::Combine_data() {
  QTest::addColumn<QString>("p1");
  QTest::addColumn<QString>("p2");
  QTest::addColumn<QString>("expected");

  QTest::newRow("no-terminating-separator")
    << "/home/user" << "hello.txt" << "/home/user/hello.txt";
  QTest::newRow("terminating-separator")
    << "/home/user/" << "hello.txt" << "/home/user/hello.txt";
}

void TestUtilities::Split_QString() {
  QFETCH(QString, str);
  QFETCH(QChar, delimiter);
  QFETCH(QList<QString>, expected);

  auto actual = Utilities::Split(str, delimiter);

  QCOMPARE(actual.size(), expected.size());
  for (size_t i = 0; i < actual.size(); ++i) {
    auto a = actual[i];
    auto e = expected[i];
    QCOMPARE(a, e);
  }
}

void TestUtilities::Split_QString_data() {
  QTest::addColumn<QString>("str");
  QTest::addColumn<QChar>("delimiter");
  QTest::addColumn<QList<QString>>("expected");

  QTest::newRow("one-space")
    << "hello world" << QChar{' '} << QList<QString>{"hello", "world"};
  QTest::newRow("two-spaces") << "hello there world" << QChar{' '}
                              << QList<QString>{"hello", "there", "world"};
  QTest::newRow("one-letter")
    << "hellozworld" << QChar{'z'} << QList<QString>{"hello", "world"};
}

void TestUtilities::Split_StdString() {
  QFETCH(QString, str);
  QFETCH(QChar, delimiter);
  QFETCH(QList<QString>, expected);

  auto actual = Utilities::Split(str.toStdString(), delimiter.toLatin1());

  QCOMPARE(actual.size(), expected.size());
  for (size_t i = 0; i < actual.size(); ++i) {
    auto a = actual[i];
    auto e = expected[i].toStdString();
    QCOMPARE(a, e);
  }
}

void TestUtilities::Split_StdString_data() {
  QTest::addColumn<QString>("str");
  QTest::addColumn<QChar>("delimiter");
  QTest::addColumn<QList<QString>>("expected");

  QTest::newRow("one-space")
    << "hello world" << QChar{' '} << QList<QString>{"hello", "world"};
  QTest::newRow("two-spaces") << "hello there world" << QChar{' '}
                              << QList<QString>{"hello", "there", "world"};
  QTest::newRow("one-letter")
    << "hellozworld" << QChar{'z'} << QList<QString>{"hello", "world"};
}
