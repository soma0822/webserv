#include "config.hpp"

Config::Config() : file_(DEFAULTCONF) {}

Config::Config(const std::string &file) : file_(file) {}

Config::Config(const Config &other)
    : file_(other.file_), server_(other.server_) {}

Config::~Config() {}

Config &Config::operator=(const Config &other) {
  if (this == &other) {
    return *this;
  }
  file_ = other.file_;
  server_ = other.server_;
  return *this;
}

// パーススタート
void Config::ParseFile() {
  std::ifstream inf(file_);
  if (!inf.is_open()) {
    throw std::invalid_argument("File could not open: " + file_);
  }
  server_ = ConfigParser::Parse(inf);
}

const std::vector<ServerContext> &Config::GetServer() const { return server_; }

std::ostream &operator<<(std::ostream &os, Config &config) {
  for (std::vector<ServerContext>::const_iterator it =
           config.GetServer().begin();
       it != config.GetServer().end(); it++) {
    os << *it << std::endl;
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const Config &config) {
  for (std::vector<ServerContext>::const_iterator it =
           config.GetServer().begin();
       it != config.GetServer().end(); it++) {
    os << *it << std::endl;
  }
  return os;
}
