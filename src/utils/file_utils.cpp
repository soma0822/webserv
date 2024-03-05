#include "file_utils.hpp"

#include <fstream>
#include <sstream>

Result<std::string, file_utils::Error> file_utils::ReadFile(
    const std::string &file_path) {
  system("pwd");
  std::ifstream ifs(file_path.c_str());
  if (!ifs.is_open()) {
    return Err(kFileOpenError);
  }

  std::stringstream ss;
  ss << ifs.rdbuf();
  return Ok(ss.str());
}
