#ifndef WEBSERV_SRC_UTILS_FILE_UTILS_HPP_
#define WEBSERV_SRC_UTILS_FILE_UTILS_HPP_

#include <string>

#include "result.hpp"

namespace file_utils {

enum Error {
  kFileOpenError = 1,
};

Result<std::string, file_utils::Error> ReadFile(const std::string &file_path);

bool IsDirectory(const std::string &path);

}  // namespace file_utils

#endif  // WEBSERV_SRC_UTILS_FILE_UTILS_HPP_
