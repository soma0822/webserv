#include "null_stream.hpp"

null_stream::NullStream::NullStream() : std::ostream(this) {}

int null_stream::NullStream::overflow(int c) { return c; }
