#include "../src/utils.h"
#include "utest.h"

UTEST(Capitalize, replaces_first_char_with_uppercase_char) {
  auto str = "bob";
  auto expected = "Bob";
  auto actual = utils::Capitalize(str);
  ASSERT_TRUE(expected == actual);
}

UTEST(Format, replaces_fmt_with_arg) {
  auto fmt = "Hello {}, how are you?";
  auto arg = "Bob";
  auto expected = "Hello Bob, how are you?";
  auto actual = utils::Format(fmt, arg);
  ASSERT_TRUE(expected == actual);
}

UTEST(Format, returns_fmt_if_fmt_has_no_fmt) {
  auto fmt = "No format";
  auto arg = "Bob";
  auto expected = fmt;
  auto actual = utils::Format(fmt, arg);
  ASSERT_TRUE(expected == actual);
}