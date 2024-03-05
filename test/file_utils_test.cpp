#include "file_utils.hpp"

#include <gtest/gtest.h>

#include <fstream>

#include "result.hpp"

TEST(FileUtilsTest, ReadFile) {
  const std::string test_file_path = "/tmp/hello.txt";
  std::string expected = "Hello, world!\n\nhi\n";
  std::ofstream ofs(test_file_path);
  ofs << expected;
  ofs.close();

  Result<std::string, file_utils::Error> result =
      file_utils::ReadFile(test_file_path);
  ASSERT_TRUE(result.IsOk());
  EXPECT_EQ(result.Unwrap(), expected);

  unlink(test_file_path.c_str());
}
