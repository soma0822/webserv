#include <iostream>

#include "config.hpp"

int main() {
  Config config;
  try {
    config.ParseFile();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
