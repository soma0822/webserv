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
void Config::parse_file() {
  std::ifstream inf(file_);
  if (!inf.is_open()) {
    throw std::invalid_argument("File could not open: " + file_);
  }
  server_ = ConfigParser::parse(inf);
}

const std::vector<ServerContext> &Config::get_server() const { return server_; }

std::ostream &operator<<(std::ostream &os, Config &config) {
  for (std::vector<ServerContext>::const_iterator it =
           config.get_server().begin();
       it != config.get_server().end(); it++) {
    os << *it << std::endl;
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const Config &config) {
  for (std::vector<ServerContext>::const_iterator it =
           config.get_server().begin();
       it != config.get_server().end(); it++) {
    os << *it << std::endl;
  }
  return os;
}
