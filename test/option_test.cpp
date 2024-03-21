#include "option.hpp"

#include <gtest/gtest.h>

TEST(OptionTest, Some) {
  Some<int> some(42);
  ASSERT_EQ(some.Unwrap(), 42);
  ASSERT_TRUE(some.IsSome());
}

TEST(OptionTest, None) {
  None<int> none;
  ASSERT_THROW(none.Unwrap(), std::runtime_error);
  ASSERT_FALSE(none.IsSome());
}

TEST(OptionTest, Option) {
  Option<int> *some = new Some(42);
  ASSERT_EQ(some->Unwrap(), 42);
  ASSERT_TRUE(some->IsSome());
  delete some;

  Option<int> *none = new None<int>();
  ASSERT_THROW(none->Unwrap(), std::runtime_error);
  ASSERT_FALSE(none->IsSome());
  delete none;
}
