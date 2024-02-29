#include "validation.hpp"

#include <gtest/gtest.h>

TEST(ValidationTest, IsNumber) {
  EXPECT_TRUE(validation::IsNumber("123"));
  EXPECT_TRUE(validation::IsNumber("0"));
  EXPECT_FALSE(validation::IsNumber("123a"));
  EXPECT_FALSE(validation::IsNumber("a123"));
  EXPECT_FALSE(validation::IsNumber("123 "));
  EXPECT_FALSE(validation::IsNumber(" 123"));
  EXPECT_FALSE(validation::IsNumber(" 123 "));
}

TEST(ValidationTest, IsPath) {
  EXPECT_TRUE(validation::IsPath("/"));
  EXPECT_TRUE(validation::IsPath("/path"));
  EXPECT_FALSE(validation::IsPath("path"));
  EXPECT_FALSE(validation::IsPath("path/"));
  EXPECT_FALSE(validation::IsPath(" /path"));
  EXPECT_FALSE(validation::IsPath(" /path "));
}

TEST(ValidationTest, IsIp) {
  EXPECT_TRUE(validation::IsIp("127.0.0.1"));
  EXPECT_TRUE(validation::IsIp("192.255.243.1"));
  EXPECT_FALSE(validation::IsIp("256.0.0.1"));
  EXPECT_FALSE(validation::IsIp("0254.0.0.1"));
  EXPECT_FALSE(validation::IsIp("127.0.1"));
}

TEST(ValidationTest, IsPort) {
  EXPECT_TRUE(validation::IsPort("0"));
  EXPECT_TRUE(validation::IsPort("65535"));
  EXPECT_FALSE(validation::IsPort("65536"));
  EXPECT_FALSE(validation::IsPort("a"));
  EXPECT_FALSE(validation::IsPort("0 "));
  EXPECT_FALSE(validation::IsPort(" 0"));
  EXPECT_FALSE(validation::IsPort(" 0 "));
}
