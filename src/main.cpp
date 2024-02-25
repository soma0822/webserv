#include <iostream>

#include "config.hpp"
#include "config_parser.hpp"

int main() {
  try {
    Config config = ConfigParser::Parse("./conf/default.conf");
    std::cout << config << std::endl;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
