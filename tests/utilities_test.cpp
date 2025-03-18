#include "utilities_test.h"

#include <QString>
#include <QTest>

#include "core/utilities.h"

void TestUtilities::RemoveAccents() {
  QFETCH(QString, input);
  QFETCH(QString, expected);

  auto actual = Utilities::RemoveAccents(input);
  QCOMPARE(actual, expected);
}

void TestUtilities::RemoveAccents_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<QString>("expected");

  QTest::newRow("no-accents") << "hello" << "hello";
  QTest::newRow("french-sentence")
    << "À Noël, où Lucía achète des canapés, Émile mange du pâté et de l'œuf, "
       "même s'il a déjà vu ça."
    << "A Noel, ou Lucia achete des canapes, Emile mange du pate et de l'œuf, "
       "meme s'il a deja vu ca.";
  QTest::newRow("russian-sentence") << "Я иду домой" << "Я иду домои";
}
