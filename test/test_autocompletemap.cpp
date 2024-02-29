#include <algorithm>
#include <cstdlib>
#include <set>
#include <string>

#include "../src/autocompletemap.h"
#include "utest.h"

UTEST_MAIN();

UTEST(Find, return_matching_sets) {
  auto autocomplete_map = AutocompleteMap();
  auto term = std::string("hello");
  autocomplete_map.Insert(term);

  auto expected = std::set<std::string>();
  expected.insert(term);

  auto key = std::string();
  for (const auto ch : term) {
    key += ch;
    auto actual = autocomplete_map.Find(key);
    ASSERT_TRUE(expected == actual);
  }
}

UTEST(Find, return_empty_set) {
  auto autocomplete_map = AutocompleteMap();
  auto term = std::string("hello");
  autocomplete_map.Insert(term);

  auto actual = autocomplete_map.Find("bye");
  ASSERT_TRUE(actual.empty());
}

UTEST(Insert, no_duplicates) {
  auto autocomplete_map = AutocompleteMap();
  auto term = "hello";
  for (size_t i = 1; i < 3; i++) {
    autocomplete_map.Insert(term);
  }

  auto expected = 1;
  auto actual = autocomplete_map.Find(term).size();
  ASSERT_EQ(expected, actual);
}