#include <gtest/gtest.h>

#include <list>
#include <vect
#include "container_to_string.hpp"

TEST(ContainerToString, VectorInt) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  std::string result;
  container_to_string(result, v);
  ASSERT_EQ("1 2 3 4 5", result);
}

TEST(ContainerToString, VectorString) {
  std::vector<std::string> v = {"Hello", "World", "!"};
  std::string result = "Tokazaki: ";
  container_to_string(result, v);
  ASSERT_EQ("Tokazaki: Hello World !", result);
}

TEST(ContainerToString, ListInt) {
  std::list<int> v = {1, 2, 3, 4, 5};
  std::string result;
  container_to_string(result, v);
  ASSERT_EQ("1 2 3 4 5", result);
}

TEST(ContainerToString, ListString) {
  std::list<std::string> v = {"Hello", "World", "!"};
  std::string result = "Tokazaki: ";
  container_to_string(result, v);
  ASSERT_EQ("Tokazaki: Hello World !", result);
}

