#ifndef WEBSERV_SRC_UTILS_TIME_HPP
#define WEBSERV_SRC_UTILS_TIME_HPP

#include <time.h>

namespace time_utils {
struct timespec GetCurrentTime();
bool TimeOut(const struct timespec &start, int timeout_s);
bool operator<(const struct timespec &lhs, const struct timespec &rhs);
}  // namespace time_utils

#endif
