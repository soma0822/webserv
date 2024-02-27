#ifndef WEBSERV_SRC_UTILS_CONTAINER_TO_STRING_HPP	
#define WEBSERV_SRC_UTILS_CONTAINER_TO_STRING_HPP

#include <iostream>

template <typename T>
void container_to_string(std::string &result, const T &container) {
  std::stringstream ss;
  ss << result;
  for (typename T::const_iterator it = container.begin(); it != container.end(); ++it) {
	ss << *it;
	if (it != --container.end()) {
	  ss << " ";
	}
  }
  result = ss.str();
}

#endif
