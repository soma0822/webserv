#include "accept.hpp"

Accept::Accept(int fd, const std::string &port, const std::string &ip, const IConfig &config)
    : AIOTask(fd, POLLIN), port_(port), ip_(ip), config_(config){}

Accept::~Accept() {}

Accept &Accept::operator=(const Accept &other) {
  (void)other;
  return *this;
}

Result<int, std::string> Accept::Execute() {
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(client_addr);
  int client_sock = accept(fd_, (struct sockaddr *)&client_addr, &len);
  if (client_sock == -1) {
    Logger::Error() << "accept エラー" << std::endl;
    return Err("accept error");
  }
  Logger::Info() << port_ << " : 接続しました" << std::endl;
   IOTaskManager::AddTask(new ReadRequestFromClient(client_sock, port_,
   ip_, config_));
  return Ok(0);
}

const std::string &Accept::GetPort() const { return port_; }
const std::string &Accept::GetIp() const { return ip_; }
