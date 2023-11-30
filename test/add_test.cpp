#include <gtest/gtest.h>
#include "example.hpp"

TEST(add_test, 1_plus_2) {
  EXPECT_EQ(3, add(1, 2));
}