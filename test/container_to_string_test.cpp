#include <gtest/gtest.h>

#include <list>
#include <vector>
#include "merge_string_and_container.hpp"

TEST(ContainerToString, VectorInt) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  ASSERT_EQ("1 2 3 4 5", MergeStringAndContainer("", v));
}

TEST(ContainerToString, VectorString) {
  std::vector<std::string> v = {"Hello", "World", "!"};
  ASSERT_EQ("Tokazaki: Hello World !", MergeStringAndContainer("Tokazaki: ", v));
}

TEST(ContainerToString, ListInt) {
  std::list<int> v = {1, 2, 3, 4, 5};
  ASSERT_EQ("1 2 3 4 5", MergeStringAndContainer("", v));
}

TEST(ContainerToString, ListString) {
  std::list<std::string> v = {"Hello", "World", "!"};
  ASSERT_EQ("Tokazaki: Hello World !", MergeStringAndContainer("Tokazaki: ", v));
}
