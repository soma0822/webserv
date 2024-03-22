#include "option.hpp"

#include <gtest/gtest.h>

TEST(OptionTest, SomeTest) {
  Option<int> opt = Some(10);
  ASSERT_EQ(opt.Unwrap(), 10);
  ASSERT_TRUE(opt.IsSome());
}

TEST(OptionTest, NoneTest) {
  Option<int> opt = None<int>();
  ASSERT_THROW(opt.Unwrap(), std::runtime_error);
  ASSERT_FALSE(opt.IsSome());
}