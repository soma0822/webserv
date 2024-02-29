#include <gtest/gtest.h>

#include <list>
#include <vector>
#include "merge_container.hpp"

TEST(ContainerToString, VectorInt) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  ASSERT_EQ("1 2 3 4 5", MergeContainer(v, " "));
}

TEST(ContainerToString, VectorString) {
  std::vector<std::string> v = {"Hello", "World", "!"};
  ASSERT_EQ("Hello World !", MergeContainer(v, " "));
}

TEST(ContainerToString, ListInt) {
  std::list<int> v = {1, 2, 3, 4, 5};
  ASSERT_EQ("1\n2\n3\n4\n5", MergeContainer(v, "\n"));
}

TEST(ContainerToString, ListString) {
  std::list<std::string> v = {"Hello", "World", "!"};
  ASSERT_EQ("Hello\nWorld\n!", MergeContainer(v, "\n"));
}
