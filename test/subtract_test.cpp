#include <gtest/gtest.h>
#include "example.hpp"

TEST(subtract_test, 2_minus_1) {
  EXPECT_EQ(1, subtract(2, 1));
}