#include "file_utils.hpp"

#include <sys/stat.h>

#include <fstream>
#include <sstream>

Result<std::string, file_utils::Error> file_utils::ReadFile(
    const std::string &file_path) {
  std::ifstream ifs(file_path.c_str());
  if (!ifs.is_open()) {
    return Err(kFileOpenError);
  }

  std::stringstream ss;
  ss << ifs.rdbuf();
  return Ok(ss.str());
}

bool file_utils::IsDirectory(const std::string &path) {
  struct stat st;
  if (stat(path.c_str(), &st) != 0) {
    return false;
  }
  return S_ISDIR(st.st_mode);
}
