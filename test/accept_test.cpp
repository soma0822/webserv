#include <gtest/gtest.h>

#include "accept.hpp"

/*
 * Acceptクラスのテスト
 * テスト前に8081ポート、9000番台ポートを使用しているプロセスをkillすること
 */
TEST(Accept, Accept) {
  std::string port = "9000";
  Accept accept(port);
  ASSERT_EQ(accept.GetPort(), "9000");
  ASSERT_EQ(accept.GetEvent(), POLL_IN);
}

TEST(Accept, AcceptError) {
  try {
    std::string port = "9001";
    Accept accept(port);
    Accept accept2(port);
  } catch (std::exception &e) {
    ASSERT_STREQ(e.what(), "9001 : bindエラー");
  }
}

//TODO: acceptのテストを書く


