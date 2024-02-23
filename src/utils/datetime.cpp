#include "datetime.hpp"

namespace datetime {
std::string GetDatetimeString() {
  time_t t = time(NULL);
  struct tm *local = localtime(&t);

  char buf[20];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", local);
  return std::string(buf);
}
} // namespace datetime