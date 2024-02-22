#include "logger.hpp"

int main() {
  NullBuffer null_buffer;
  std::ostream null_stream(&null_buffer);
  null_stream << "Info message" << std::endl;
  null_stream << "Warn message" << std::endl;
  null_stream << "Error message" << std::endl;
  null_stream << "Debug message" << std::endl;
  return 0;
}