#include <iostream>
#include "config.hpp"

int main() {
  Config config;
  try{
    config.parse_file();
  } catch (std::exception){
    return 1;
  }
}