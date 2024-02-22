#include "logger.hpp"

int main() {
  Logger::SetLogLevel(kWarn);
  Logger::Info() << "Info message" << std::endl;
  Logger::Warn() << "Warn message" << std::endl;
  Logger::Error() << "Error message" << std::endl;
  Logger::Debug() << "Debug message" << std::endl;
  return 0;
}