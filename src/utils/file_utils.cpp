#include "file_utils.hpp"

#include <dirent.h>
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

bool file_utils::IsFile(const std::string &path) {
  struct stat st;
  if (stat(path.c_str(), &st) != 0) {
    return false;
  }
  return S_ISREG(st.st_mode);
}

// ディレクトリに対する実行権限を必要とする
bool file_utils::DoesFileOrDirectoryExist(const std::string &path) {
  return access(path.c_str(), F_OK) == 0;
}

// 実行権限を必要としないでファイルが存在するかどうかを確認する。読み取り権限は必要
// 絶対パスで渡す必要がある
bool file_utils::CheckIfFileExistsWithoutExecPermission(
    const std::string &path) {
  const std::string parent_dir = path.substr(0, path.find_last_of('/'));

  DIR *dir = opendir(parent_dir.c_str());
  if (!dir) {
    closedir(dir);
    return false;
  }
  for (dirent *dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
    if (parent_dir + "/" + dp->d_name == path) {
      closedir(dir);
      return true;
    }
  }
  closedir(dir);
  return false;
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
