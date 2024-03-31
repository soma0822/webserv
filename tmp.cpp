#include <arpa/inet.h>
#include <iostream>


// 32ビット整数のIPv4アドレスをドット区切りの10進数表現に変換する関数
std::string InetNtoa(unsigned int ip) {
    std::string ret;
    for (unsigned int i = 0; i < 4; i++){
      ret += std::to_string(ip >> (i * 8) & 255);
      if (i != 3) 
        ret += ".";
    }
    return ret;
}

int main() {
    in_addr address;
    std::string ip_str;\
    ip_str = "127.0.0.1";
    address.s_addr = inet_addr(ip_str.c_str());
    std::cout << "s_addr: " << address.s_addr << std::endl;
    std::cout << "inet_ntoa: " << inet_ntoa(address) << std::endl;
    std::cout << "InetNtoa: " << InetNtoa(address.s_addr) << std::endl;
    ip_str = "255.255.255.255";
    address.s_addr = inet_addr(ip_str.c_str());
    std::cout << "inet_ntoa: " << inet_ntoa(address) << std::endl;
    std::cout << "InetNtoa: " << InetNtoa(address.s_addr) << std::endl;
    ip_str = "192.60.7.1";
    address.s_addr = inet_addr(ip_str.c_str());
    std::cout << "inet_ntoa: " << inet_ntoa(address) << std::endl;
    std::cout << "InetNtoa: " << InetNtoa(address.s_addr) << std::endl;
    ip_str = "3.4.4.4";
    address.s_addr = inet_addr(ip_str.c_str());
    std::cout << "inet_ntoa: " << inet_ntoa(address) << std::endl;
    std::cout << "InetNtoa: " << InetNtoa(address.s_addr) << std::endl;
    return 0;
}
