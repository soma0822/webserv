#ifndef WEBSERV_SRC_UTILS_MERGE_CONTAINER_HPP
#define WEBSERV_SRC_UTILS_MERGE_CONTAINER_HPP

#include <iostream>

// コンテナと区切り文字を受け取り、コンテナの要素を区切り文字で結合した文字列を返す
namespace container {
template <typename T>
std::string MergeContainer(const T &container, const std::string &sed) {
  std::stringstream ss;
  std::string result;
  for (typename T::const_iterator it = container.begin(); it != container.end();
       ++it) {
    ss << *it;
    if (it != --container.end()) {
      ss << sed;
    }
  }
  result = ss.str();
  return result;
}
}  // namespace container

#endif
