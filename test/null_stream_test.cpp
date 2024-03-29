#include "null_stream.hpp"

#include <gtest/gtest.h>

TEST(NullStreamTest, NothingPrintedToStdout) {
  null_stream::NullStream null_stream;

  testing::internal::CaptureStdout();
  ASSERT_STRCASEEQ("", testing::internal::GetCapturedStdout().c_str());
}

TEST(NullStreamTest, NothingPrintedToStderr) {
  null_stream::NullStream null_stream;

  testing::internal::CaptureStderr();
  ASSERT_STRCASEEQ("", testing::internal::GetCapturedStderr().c_str());
}