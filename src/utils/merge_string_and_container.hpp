#ifndef WEBSERV_SRC_UTILS_MERGE_STRING_AND_CONTAINER_HPP
#define WEBSERV_SRC_UTILS_MERGE_STRING_AND_CONTAINER_HPP

#include <iostream>

template <typename T>
std::string MergeStringAndContainer(const std::string &str,
                                    const T &container) {
  std::stringstream ss;
  std::string result;
  ss << str;
  for (typename T::const_iterator it = container.begin(); it != container.end();
       ++it) {
    ss << *it;
    if (it != --container.end()) {
      ss << " ";
    }
  }
  result = ss.str();
  return result;
}

#endif
