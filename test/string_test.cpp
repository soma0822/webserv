#include <gtest/gtest.h>

#include "string_utils.hpp"

TEST(STRINGSTOI, Positive) {
  std::string str = "123";
  Result<int, std::string> result = string_utils::StrToI(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 123);
}

TEST(STRINGSTOI, INTMAX) {
  std::string str = "2147483647";
  Result<int, std::string> result = string_utils::StrToI(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 2147483647);
}

TEST(STRINGSTOI, Zero) {
  std::string str = "0";
  Result<int, std::string> result = string_utils::StrToI(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 0);
}

TEST(STRINGSTOI, INTMAXPlus1) {
  std::string str = "2147483648";
  Result<int, std::string> result = string_utils::StrToI(str);
  ASSERT_TRUE(result.IsErr());
}

TEST(STRINGSTOHEX, Positive) {
  std::string str = "7B";
  Result<int, std::string> result = string_utils::StrToHex(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 123);
}

TEST(STRINGSTOHEX, INTMAX) {
  std::string str = "7FFFFFFF";
  Result<int, std::string> result = string_utils::StrToHex(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 2147483647);
}

TEST(STRINGSTOHEX, Zero) {
  std::string str = "0";
  Result<int, std::string> result = string_utils::StrToHex(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 0);
}

TEST(STRINGSTOHEX, INTMAXPlus1) {
  std::string str = "80000000";
  Result<int, std::string> result = string_utils::StrToHex(str);
  ASSERT_TRUE(result.IsErr());
}

TEST(Capitalize, OneWord) {
  std::string target = "hello";
  ASSERT_EQ("Hello", string_utils::CapitalizeHyphenSeparatedWords(target));
}

TEST(Capitalize, TwoWords) {
  std::string target = "hello-there";
  ASSERT_EQ("Hello-There", string_utils::CapitalizeHyphenSeparatedWords(target));
}
