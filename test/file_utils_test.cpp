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

TEST(FileUtilsTest, IsDirectoryDirectory) {
  const std::string test_dir_path = "/tmp/test_dir";
  mkdir(test_dir_path.c_str(), 0755);

  ASSERT_TRUE(file_utils::IsDirectory(test_dir_path));

  rmdir(test_dir_path.c_str());
}

TEST(FileUtilsTest, IsDirectoryFile) {
  const std::string test_file_path = "/tmp/test_file";
  std::ofstream ofs(test_file_path);
  ofs << "Hello, world!";
  ofs.close();

  ASSERT_FALSE(file_utils::IsDirectory(test_file_path));

  unlink(test_file_path.c_str());
}
