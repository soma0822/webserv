#include <gtest/gtest.h>

#include "logger.hpp"

TEST(Logger, InfoStdOut) {
  Logger::SetHandler(new StdoutStreamWrapper());
  testing::internal::CaptureStdout();
  Logger::Info() << "This is an info message" << std::endl;
  ASSERT_STRCASEEQ("[Info] This is an info message\n",
                   testing::internal::GetCapturedStdout().c_str());
}

TEST(Logger, WarnStdOut) {
  Logger::SetHandler(new StdoutStreamWrapper());
  testing::internal::CaptureStdout();
  Logger::Warn() << "This is a warning message" << std::endl;
  ASSERT_STRCASEEQ("[Warn] This is a warning message\n",
                   testing::internal::GetCapturedStdout().c_str());
}

TEST(Logger, ErrorStdOut) {
  Logger::SetHandler(new StdoutStreamWrapper());
  testing::internal::CaptureStdout();
  Logger::Error() << "This is an error message" << std::endl;
  ASSERT_STRCASEEQ("[Error] This is an error message\n",
                   testing::internal::GetCapturedStdout().c_str());
}

TEST(Logger, DebugStdOut) {
  Logger::SetHandler(new StdoutStreamWrapper());
  testing::internal::CaptureStdout();
  Logger::Debug() << "This is a debug message" << std::endl;
  ASSERT_STRCASEEQ("[Debug] This is a debug message\n",
                   testing::internal::GetCapturedStdout().c_str());
}

TEST(Logger, InfoFile) {
  Logger::SetHandler(new FileStreamWrapper("test.log"));
  Logger::Info() << "This is an info message" << std::endl;
  Logger::SetHandler(new StdoutStreamWrapper());
  std::ifstream file("test.log");
  std::string line;
  std::getline(file, line);
  ASSERT_STRCASEEQ("[Info] This is an info message", line.c_str());
  file.close();
  remove("test.log");
}

TEST(Logger, WarnFile) {
  Logger::SetHandler(new FileStreamWrapper("test.log"));
  Logger::Warn() << "This is a warning message" << std::endl;
  Logger::SetHandler(new StdoutStreamWrapper());
  std::ifstream file("test.log");
  std::string line;
  std::getline(file, line);
  ASSERT_STRCASEEQ("[Warn] This is a warning message", line.c_str());
  file.close();
  remove("test.log");
}

TEST(Logger, ErrorFile) {
  Logger::SetHandler(new FileStreamWrapper("test.log"));
  Logger::Error() << "This is an error message" << std::endl;
  Logger::SetHandler(new StdoutStreamWrapper());
  std::ifstream file("test.log");
  std::string line;
  std::getline(file, line);
  ASSERT_STRCASEEQ("[Error] This is an error message", line.c_str());
  file.close();
  remove("test.log");
}

TEST(Logger, DebugFile) {
  Logger::SetHandler(new FileStreamWrapper("test.log"));
  Logger::Debug() << "This is a debug message" << std::endl;
  Logger::SetHandler(new StdoutStreamWrapper());
  std::ifstream file("test.log");
  std::string line;
  std::getline(file, line);
  ASSERT_STRCASEEQ("[Debug] This is a debug message", line.c_str());
  file.close();
  remove("test.log");
}

TEST(Logger, SetLogLevelInfo) {
  Logger::SetHandler(new StdoutStreamWrapper());
  Logger::SetLogLevel(kInfo);

  // Test info message
  testing::internal::CaptureStdout();
  Logger::Info() << "This is an info message" << std::endl;
  ASSERT_STRCASEEQ("[Info] This is an info message\n",
                   testing::internal::GetCapturedStdout().c_str());
}

TEST(Logger, SetLogLevelWarn) {
  Logger::SetHandler(new StdoutStreamWrapper());
  Logger::SetLogLevel(kWarn);

  // Test info message
  testing::internal::CaptureStdout();
  Logger::Info() << "This is an info message" << std::endl;
  ASSERT_STRCASEEQ("", testing::internal::GetCapturedStdout().c_str());

  // Test warn message
  testing::internal::CaptureStdout();
  Logger::Warn() << "This is a warning message" << std::endl;
  ASSERT_STRCASEEQ("[Warn] This is a warning message\n",
                   testing::internal::GetCapturedStdout().c_str());
}

TEST(Logger, SetLogLevelError) {
  Logger::SetHandler(new StdoutStreamWrapper());
  Logger::SetLogLevel(kError);

  // Test info message
  testing::internal::CaptureStdout();
  Logger::Info() << "This is an info message" << std::endl;
  ASSERT_STRCASEEQ("", testing::internal::GetCapturedStdout().c_str());

  // Test warn message
  testing::internal::CaptureStdout();
  Logger::Warn() << "This is a warning message" << std::endl;
  ASSERT_STRCASEEQ("", testing::internal::GetCapturedStdout().c_str());

  // Test error message
  testing::internal::CaptureStdout();
  Logger::Error() << "This is an error message" << std::endl;
  ASSERT_STRCASEEQ("[Error] This is an error message\n",
                   testing::internal::GetCapturedStdout().c_str());
}

TEST(Logger, SetLogLevelDebug) {
  Logger::SetHandler(new StdoutStreamWrapper());
  Logger::SetLogLevel(kDebug);

  // Test info message
  testing::internal::CaptureStdout();
  Logger::Info() << "This is an info message" << std::endl;
  ASSERT_STRCASEEQ("", testing::internal::GetCapturedStdout().c_str());

  // Test warn message
  testing::internal::CaptureStdout();
  Logger::Warn() << "This is a warning message" << std::endl;
  ASSERT_STRCASEEQ("", testing::internal::GetCapturedStdout().c_str());

  // Test error message
  testing::internal::CaptureStdout();
  Logger::Error() << "This is an error message" << std::endl;
  ASSERT_STRCASEEQ("", testing::internal::GetCapturedStdout().c_str());

  // Test debug message
  testing::internal::CaptureStdout();
  Logger::Debug() << "This is a debug message" << std::endl;
  ASSERT_STRCASEEQ("[Debug] This is a debug message\n",
                   testing::internal::GetCapturedStdout().c_str());
}