#include "validation.hpp"

namespace validation {

bool IsNumber(const std::string &str) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (!std::isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

bool IsHexNumber(const std::string &str) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (!std::isxdigit(str[i])) {
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
  for (size_t i = 0; i < parts.size(); ++i) {
    if (parts[i].size() > 3) {
      return false;
    }
    Result<int, std::string> result = string_utils::StrToI(parts[i]);
    if (result.IsErr()) return false;
    int num = result.Unwrap();
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
  Result<int, std::string> result = string_utils::StrToI(str);
  if (result.IsErr()) return false;
  int num = result.Unwrap();
  if (num < 0 || num > kMaxPort) {
    return false;
  }
  return true;
}

}  // namespace validation
