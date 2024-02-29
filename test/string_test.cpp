#include <gtest/gtest.h>

#include "string.hpp"

TEST(STRINGSTOI, Positive) {
  std::string str = "123";
  Result<int, std::string> result = string::StrToI(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 123);
}

TEST(STRINGSTOI, INTMAX) {
  std::string str = "2147483647";
  Result<int, std::string> result = string::StrToI(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 2147483647);
}

TEST(STRINGSTOI, Zero) {
  std::string str = "0";
  Result<int, std::string> result = string::StrToI(str);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 0);
}

TEST(STRINGSTOI, INTMAXPlus1) {
  std::string str = "2147483648";
  Result<int, std::string> result = string::StrToI(str);
  ASSERT_TRUE(result.IsErr());
  ASSERT_EQ(result.UnwrapErr(), "2147483648 is too large");
}
