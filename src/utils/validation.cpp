#include "validation.hpp"

namespace validation {

bool IsNumber(const std::string &str) {
  for (size_t i = 0; i < str.size(); i++) {
	if (!std::isdigit(str[i])) {
	  return false;
	}
  }
  return true;
}

bool IsPath(const std::string &str) {
  if (str[0] != '/') {
	return false;
  }
  return true;
}

bool IsIp(const std::string &str) {
  	std::vector<std::string> parts;
	std::istringstream iss(str);
    std::string part;

    while (std::getline(iss, part, '.')) {
        parts.push_back(part);
    }
  if (parts.size() != 4) {
	return false;
  }
  for (size_t i = 0; i < parts.size(); i++) {
	if (!IsNumber(parts[i]) || parts[i].size() > 3) {
	  return false;
	}
	int num = std::stoi(parts[i]);
	if (num < 0 || num > 255) {
	  return false;
	}
  }
  return true;
}

bool IsPort(const std::string &str) {
  if (!IsNumber(str)) {
	return false;
  }
  int num = std::stoi(str);
  if (num < 0 || num > kMaxPort) {
	return false;
  }
  return true;
}

}  // namespace validation
