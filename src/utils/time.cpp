#include "time.hpp"

struct timespec time::GetCurrentTime() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts;
}

bool time::TimeOut(const struct timespec &start, int timeout_s) {
  struct timespec now = GetCurrentTime();
  struct timespec timeout;
  timeout.tv_sec = start.tv_sec + timeout_s;
  timeout.tv_nsec = start.tv_nsec;
  return timeout < now;
}

bool time::operator<(const struct timespec &lhs, const struct timespec &rhs) {
  if (lhs.tv_sec < rhs.tv_sec) {
    return true;
  }
  if (lhs.tv_sec > rhs.tv_sec) {
    return false;
  }
  return lhs.tv_nsec < rhs.tv_nsec;
}
