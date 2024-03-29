#include <gtest/gtest.h>

#include <list>
#include <vector>

#include "container.hpp"

TEST(ContainerMerge, VectorInt) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  ASSERT_EQ("1 2 3 4 5", container::MergeContainer(v, " "));
}

TEST(ContainerMerge, VectorString) {
  std::vector<std::string> v = {"Hello", "World", "!"};
  ASSERT_EQ("Hello World !", container::MergeContainer(v, " "));
}

TEST(ContainerMerge, ListInt) {
  std::list<int> v = {1, 2, 3, 4, 5};
  ASSERT_EQ("1\n2\n3\n4\n5", container::MergeContainer(v, "\n"));
}

TEST(ContainerMerge, ListString) {
  std::list<std::string> v = {"Hello", "World", "!"};
  ASSERT_EQ("Hello\nWorld\n!", container::MergeContainer(v, "\n"));
}
