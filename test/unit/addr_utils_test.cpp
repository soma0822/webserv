#include "addr_utils.hpp"

#include <arpa/inet.h>
#include <gtest/gtest.h>

#include "cgi_handler.hpp"

TEST(AddrUtils, InetAddr) {
  std::string ip = "127.0.0.1";
  ASSERT_EQ(inet_addr(ip.c_str()), addr_utils::InetAddr(ip));
  ip = "192.168.64.2";
  ASSERT_EQ(inet_addr(ip.c_str()), addr_utils::InetAddr(ip));
  ip = "192.168.64";
  ASSERT_EQ(0, addr_utils::InetAddr(ip));
  ip = "192.255.64.2";
  ASSERT_EQ(inet_addr(ip.c_str()), addr_utils::InetAddr(ip));
}
