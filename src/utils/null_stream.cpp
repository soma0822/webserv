#include "null_stream.hpp"

static null_stream::NullBuffer null_buffer_;
std::ostream null_stream::null_stream(&null_buffer_);

int null_stream::NullBuffer::overflow(int c) { return c; }
