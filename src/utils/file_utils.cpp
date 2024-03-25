#include "file_utils.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

std::string file_utils::ReadFile(const std::string &file_path) {
  std::ifstream ifs(file_path.c_str());

  std::stringstream ss;
  ss << ifs.rdbuf();
  return ss.str();
}

bool file_utils::IsDirectory(const std::string &path) {
  struct stat st;
  if (stat(path.c_str(), &st) != 0) {
    return false;
  }
  return S_ISDIR(st.st_mode);
}

bool file_utils::DoesFileExist(const std::string &path) {
  return access(path.c_str(), F_OK) == 0;
}

bool file_utils::IsReadable(const std::string &path) {
  return access(path.c_str(), R_OK) == 0;
}

bool file_utils::IsWritable(const std::string &path) {
  return access(path.c_str(), W_OK) == 0;
}

bool file_utils::IsExecutable(const std::string &path) {
  return access(path.c_str(), X_OK) == 0;
}
