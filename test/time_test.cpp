#include <gtest/gtest.h>
#include "time.hpp"
#include <unistd.h>

TEST(TIME, GetCurrentTime) {
  struct timespec ts = time_utils::GetCurrentTime();
  sleep(1);
  struct timespec ts2 = time_utils::GetCurrentTime();
  ASSERT_TRUE(ts.tv_sec < ts2.tv_sec);
  ASSERT_TRUE(time_utils::TimeOut(ts, 1));
  ASSERT_FALSE(time_utils::TimeOut(ts2, 1));
  ASSERT_FALSE(time_utils::TimeOut(ts, 10));
}
