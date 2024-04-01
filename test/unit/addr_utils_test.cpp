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

TEST(AddrUtils, InetNtoa) {
  in_addr address;
  std::string ip_str;
  ip_str = "127.0.0.1";
  address.s_addr = inet_addr(ip_str.c_str());
  ASSERT_EQ(inet_ntoa(address), addr_utils::InetNtoa(address.s_addr));
  ip_str = "255.255.255.255";
  address.s_addr = inet_addr(ip_str.c_str());
  ASSERT_EQ(inet_ntoa(address), addr_utils::InetNtoa(address.s_addr));
  ip_str = "192.60.7.1";
  address.s_addr = inet_addr(ip_str.c_str());
  ASSERT_EQ(inet_ntoa(address), addr_utils::InetNtoa(address.s_addr));
  ip_str = "3.4.4.4";
  address.s_addr = inet_addr(ip_str.c_str());
  ASSERT_EQ(inet_ntoa(address), addr_utils::InetNtoa(address.s_addr));
}
