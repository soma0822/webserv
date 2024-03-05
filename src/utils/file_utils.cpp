#include "file_utils.hpp"

#include <fstream>

Result<std::string, file_utils::Error> file_utils::ReadFile(
    const std::string &file_path) {
  std::ifstream ifs(file_path.c_str());
  if (!ifs.is_open()) {
    return Err(kFileOpenError);
  }
  return Ok(std::string((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>())));
}
