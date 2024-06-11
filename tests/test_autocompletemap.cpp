#include <QString>
#include <algorithm>
#include <cstdlib>
#include <set>

#include "../src/core/autocompleter.h"
#include "../src/core/input.h"
#include "utest.h"

UTEST_MAIN();

UTEST(Find, return_matching_sets) {
  auto autocompleter = Autocompleter();
  auto term = QString("hello");
  autocompleter.Insert(term);

  auto expected = std::set<QString>();
  expected.insert(term);

  auto key = QString();
  for (const auto ch : term) {
    key += ch;
    auto actual = autocompleter.Find(Input{key});
    ASSERT_TRUE(expected == actual);
  }
}

UTEST(Find, return_empty_set) {
  auto autocompleter = Autocompleter();
  auto term = QString("hello");
  autocompleter.Insert(term);

  auto actual = autocompleter.Find(Input{"bye"});
  ASSERT_TRUE(actual.empty());
}

UTEST(Insert, no_duplicates) {
  auto autocompleter = Autocompleter();
  auto term = QString("hello");
  for (size_t i = 1; i < 3; i++) {
    autocompleter.Insert(term);
  }

  auto expected = 1;
  auto actual = autocompleter.Find(Input{term}).size();
  ASSERT_EQ(expected, actual);
}
