#include <gtest/gtest.h>

#include "result.hpp"

namespace {
Result<std::string, std::string> IsEven(int a) {
  if (a % 2 != 0) {
    return Err("Not an even number");
  }
  return Ok("Even number");
}
} // namespace

// 処理が成功した場合
TEST(ResultTest, IsEven) {
  Result<std::string, std::string> result = IsEven(10);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), "Even number");
}

// 処理が失敗した場合
TEST(ResultTest, IsNotEven) {
  Result<std::string, std::string> result = IsEven(11);
  ASSERT_TRUE(result.IsErr());
  ASSERT_EQ(result.UnwrapErr(), "Not an even number");
}