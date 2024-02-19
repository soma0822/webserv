#include <gtest/gtest.h>

#include "result.hpp"

namespace {
Result<std::string, std::string> ProcessSomething(bool is_success) {
  if (is_success) {
    return Ok("Success");
  }
  return Err("Failure");
}

Result<int, std::string> OpenFile(bool is_success) {
  if (is_success) {
    return Ok(10);
  }
  return Err("File not found");
}
} // namespace

/*
 * Resultクラスのテスト
 */

// コンストラクタ
TEST(ResultTest, ResultConstructor) {
  Result<int, std::string> result(10, "", true);
  ASSERT_EQ(result.Unwrap(), 10);
}

// コピーコンストラクタ
TEST(ResultTest, ResultCopyConstructor) {
  Result<int, std::string> result(10, "", true);
  Result<int, std::string> result2(result);
  ASSERT_EQ(result2.Unwrap(), 10);
}

// コピー代入演算子
TEST(ResultTest, ResultCopyAssignmentOperator) {
  Result<int, std::string> result(10, "", true);
  Result<int, std::string> result2 = result;
  ASSERT_EQ(result2.Unwrap(), 10);
}

// 処理が成功した場合
// OkTとErrTが同じ型の場合
TEST(ResultTest, ResultSuccessWithSameType) {
  Result<std::string, std::string> result = ProcessSomething(true);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), "Success");
}

// OkTとErrTが異なる型の場合
TEST(ResultTest, ResultSuccessWithDifferentType) {
  Result<int, std::string> result = OpenFile(true);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 10);
}

// 処理が失敗した場合
// OkTとErrTが同じ型の場合
TEST(ResultTest, ResultFailureWithSameType) {
  Result<std::string, std::string> result = ProcessSomething(false);
  ASSERT_TRUE(result.IsErr());
  ASSERT_EQ(result.UnwrapErr(), "Failure");
}

// OkTとErrTが異なる型の場合
TEST(ResultTest, ResultFailureWithDifferentType) {
  Result<int, std::string> result = OpenFile(false);
  ASSERT_TRUE(result.IsErr());
  ASSERT_EQ(result.UnwrapErr(), "File not found");
}

// IsErrがtrueの場合にUnwrapを呼ぶと例外を投げる
TEST(ResultTest, CallUnwrapOnErrCauseException) {
  Result<std::string, std::string> result = ProcessSomething(false);
  ASSERT_TRUE(result.IsErr());
  ASSERT_THROW(result.Unwrap(), std::runtime_error);
  // 例外のメッセージを確認する
  try {
    result.Unwrap();
  } catch (const std::runtime_error &e) {
    ASSERT_STREQ(e.what(), "Result does not have a value");
  }
}

// IsOkがtrueの場合にUnwrapErrを呼ぶと例外を投げる
TEST(ResultTest, CallUnwrapErrOnOkCauseException) {
  Result<std::string, std::string> result = ProcessSomething(false);
  ASSERT_NO_THROW(result.UnwrapErr());
  // 例外のメッセージを確認する
  try {
    result.UnwrapErr();
  } catch (const std::runtime_error &e) {
    ASSERT_STREQ(e.what(), "Result does not have an error");
  }
}

/*
 * Ok, Errヘルパー関数のテスト
 */

// Okヘルパー関数のテスト
TEST(ResultTest, OkHelper) {
  Result<int, std::string> result = Ok(10);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 10);
}

// Errヘルパー関数のテスト
TEST(ResultTest, ErrHelper) {
  Result<int, std::string> result = Err("Error");
  ASSERT_TRUE(result.IsErr());
  ASSERT_EQ(result.UnwrapErr(), "Error");
}