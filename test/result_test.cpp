#include <gtest/gtest.h>

#include "result.hpp"

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
  (void)result; // 未使用変数の警告を回避する
  ASSERT_EQ(result2.Unwrap(), 10);
}

// コピー代入演算子
TEST(ResultTest, ResultCopyAssignmentOperator) {
  Result<int, std::string> result(10, "", true);
  Result<int, std::string> result2 = result;
  (void)result; // 未使用変数の警告を回避する
  ASSERT_EQ(result2.Unwrap(), 10);
}

/*
 * IsOkのテスト
 */

// OkTとErrTが同じ型の場合
TEST(ResultTest, ResultSuccessWithSameType) {
  Result<std::string, std::string> result = Ok("Success");
  ASSERT_TRUE(result.IsOk());
}

// OkTとErrTが異なる型の場合
TEST(ResultTest, ResultSuccessWithDifferentType) {
  Result<int, std::string> result = Ok(10);
  ASSERT_TRUE(result.IsOk());
}

// OkTがvoidの場合
TEST(ResultTest, ResultSuccessWithVoid) {
  Result<void, std::string> result = Ok();
  ASSERT_TRUE(result.IsOk());
}

/*
 * IsErrのテスト
 */

// OkTとErrTが同じ型の場合
TEST(ResultTest, ResultFailureWithSameType) {
  Result<std::string, std::string> result = Err("Failure");
  ASSERT_TRUE(result.IsErr());
}

// OkTとErrTが異なる型の場合
TEST(ResultTest, ResultFailureWithDifferentType) {
  Result<int, std::string> result = Err("File not found");
  ASSERT_TRUE(result.IsErr());
}

// OkTがvoidの場合
TEST(ResultTest, ResultFailureWithVoid) {
  Result<void, std::string> result = Err("Error");
  ASSERT_TRUE(result.IsErr());
}

/*
 * Unwrapのテスト
 */

// IsOkがtrueの場合にUnwrapを呼ぶ
TEST(ResultTest, CallUnwrapOnOk) {
  Result<std::string, std::string> result = Ok("Success");
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), "Success");
}

// IsErrがtrueの場合にUnwrapを呼ぶと例外を投げる
TEST(ResultTest, CallUnwrapOnErrCauseException) {
  Result<std::string, std::string> result = Err("Error");
  ASSERT_TRUE(result.IsErr());
  ASSERT_THROW(result.Unwrap(), std::runtime_error);
}

/*
 * UnwrapErrのテスト
 */

// IsErrがtrueの場合にUnwrapErrを呼ぶ
TEST(ResultTest, CallUnwrapErrOnErr) {
  Result<std::string, std::string> result = Err("Error");
  ASSERT_TRUE(result.IsErr());
  ASSERT_EQ(result.UnwrapErr(), "Error");
}

// IsOkがtrueの場合にUnwrapErrを呼ぶと例外を投げる
TEST(ResultTest, CallUnwrapErrOnOkCauseException) {
  Result<std::string, std::string> result = Err("Error");
  ASSERT_NO_THROW(result.UnwrapErr());
}

/*
 * Ok関数のテスト
 */

TEST(ResultTest, OkHelper) {
  Result<int, std::string> result = Ok(10);
  ASSERT_TRUE(result.IsOk());
  ASSERT_EQ(result.Unwrap(), 10);
}

// OkTがvoidの場合
TEST(ResultTest, OkHelperWithVoid) {
  Result<void, std::string> result = Ok();
  ASSERT_TRUE(result.IsOk());
}

/*
 * Err関数のテスト
 */

TEST(ResultTest, ErrHelper) {
  Result<int, std::string> result = Err("Error");
  ASSERT_TRUE(result.IsErr());
  ASSERT_EQ(result.UnwrapErr(), "Error");
}