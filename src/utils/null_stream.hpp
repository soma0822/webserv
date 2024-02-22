#ifndef WEVSERV_SRC_UTILS_NULL_STREAM_HPP_
#define WEVSERV_SRC_UTILS_NULL_STREAM_HPP_

#include <iostream>
#include <streambuf>

namespace null_stream {
extern std::ostream null_stream;

class NullBuffer : public std::streambuf {
public:
  int overflow(int c);
};
} // namespace null_stream

#endif // WEVSERV_SRC_UTILS_NULL_STREAM_HPP_
