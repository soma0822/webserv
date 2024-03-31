#include "addr_utils.hpp"

uint32_t addr_utils::InetAddr(const std::string& ip_str) {
  std::vector<uint8_t> ip_bytes;
  std::stringstream ss(ip_str);
  std::string token;
  while (std::getline(ss, token, '.')) {
    Result<int, std::string> result = string_utils::StrToI(token);
    if (result.IsErr()) return 0;
    ip_bytes.push_back(result.Unwrap());
  }
  if (ip_bytes.size() != 4) {
    return 0;
  }

  uint32_t ip_addr = 0;
  for (int i = 0; i < 4; ++i) {
    ip_addr += ip_bytes[3 - i] << (8 * (3 - i));
  }
  return ip_addr;
}

// 32ビット整数のIPv4アドレスをドット区切りの10進数表現に変換する関数
std::string addr_utils::InetNtoa(uint32_t ip) {
  std::string ret;
  for (unsigned int i = 0; i < 4; i++) {
    std::stringstream ss;
    ss << (ip >> (i * 8) & 255);
    std::string tmp;
    ss >> tmp;
    ret += tmp;
    if (i != 3) ret += ".";
  }
  return ret;
}
