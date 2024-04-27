#include <cctype>
#include <string>
#include <vector>

#include "../src/core/utils.h"
#include "utest.h"

UTEST(Capitalize, replaces_first_char_with_uppercase_char) {
  auto str = "bob";
  auto expected = "Bob";
  auto actual = utils::Capitalize(str);
  ASSERT_TRUE(expected == actual);
}

UTEST(CompareString, returns_false) {
  auto str1 = std::string{"a"};
  auto str2 = std::string{"b"};
  ASSERT_FALSE(utils::CompareStrings(str2, str1));
}

UTEST(CompareString, returns_false_with_reverse) {
  auto str1 = std::string{"a"};
  auto str2 = std::string{"b"};
  ASSERT_FALSE(utils::CompareStrings(str1, str2, true));
}

UTEST(CompareString, returns_true) {
  auto str1 = std::string{"a"};
  auto str2 = std::string{"b"};
  ASSERT_TRUE(utils::CompareStrings(str1, str2));
}

UTEST(CompareString, returns_true_with_reverse) {
  auto str1 = std::string{"a"};
  auto str2 = std::string{"b"};
  ASSERT_TRUE(utils::CompareStrings(str2, str1, true));
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

UTEST(ToLower, returns_lowercase_characters) {
  auto ascii_uppercase = std::string{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  for (auto ch : ascii_uppercase) {
    EXPECT_TRUE_MSG(std::tolower(ch) == utils::ToLower(ch), &ch);
  }
}

UTEST(Split, string_with_multiple_tokens) {
  auto str = std::string{"hello:there:world"};
  const auto expected = std::vector<std::string>{"hello", "there", "world"};
  auto actual = utils::Split(str, ':');
  for (size_t i = 0; i < expected.size(); ++i) {
    ASSERT_TRUE(expected[i] == actual[i]);
  }
}

UTEST(Split, string_with_no_tokens) {
  auto str = std::string{"hello world"};
  const auto expected = std::vector<std::string>{"hello world"};
  auto actual = utils::Split(str, ':');
  for (size_t i = 0; i < expected.size(); ++i) {
    ASSERT_TRUE(expected[i] == actual[i]);
  }
}

UTEST(Split, string_with_one_token) {
  auto str = std::string{"hello:world"};
  const auto expected = std::vector<std::string>{"hello", "world"};
  auto actual = utils::Split(str, ':');
  for (size_t i = 0; i < expected.size(); ++i) {
    ASSERT_TRUE(expected[i] == actual[i]);
  }
}

UTEST(Strip, removes_spaces) {
  auto input = std::string{"  hello world  "};
  auto expected = std::string{"hello world"};
  ASSERT_STREQ(expected.c_str(), utils::Strip(input).c_str());
}

UTEST(Strip, removes_additional_char) {
  auto input = std::string{"'hello world'"};
  auto expected = std::string{"hello world"};
  ASSERT_STREQ(expected.c_str(), utils::Strip(input, '\'').c_str());
}
