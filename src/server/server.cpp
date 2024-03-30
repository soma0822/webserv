#include "server.hpp"

Server::Server() {}

Server::~Server() {}

Server &Server::operator=(const Server &other) {
  (void)other;
  return *this;
}

bool Server::Run(const IConfig &config) {
  const std::vector<ServerContext> &servers = config.GetServer();
  std::vector<ServerContext>::const_iterator server_it = servers.begin();
  std::map<std::string, std::set<std::string> > listen_port;
  for (; server_it != servers.end(); ++server_it) {
    if (listen_port[server_it->GetPort()].insert(server_it->GetIp()).second) {
      Result<int, int> result =
          Listen(server_it->GetPort(), server_it->GetIp());
      if (result.IsOk()) {
        IOTaskManager::AddTask(new Accept(result.Unwrap(), server_it->GetPort(),
                                          server_it->GetIp(), config));
      } else {
        Logger::Error() << "リッスンに失敗しました" << std::endl;
        IOTaskManager::DeleteTasks();
        return false;
      }
    }
  }
  IOTaskManager::ExecuteTasks();
  return true;
}

Result<int, int> Server::Listen(const std::string &port,
                                const std::string &ip) {
  int optval = 1;
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) return Err(kSocketError);
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    return Err(kSetSockOptError);
  struct sockaddr_in addr;
  Result<int, std::string> result = string_utils::StrToI(port);

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(result.Unwrap());
  // TODO:
  // inet_addrは使用可能ではないため、自作の必要あり　空文字列の場合INADDR_ANYを返してくる
  uint32_t s_addr = InetAddr(ip.c_str());
  if (s_addr == 0)
    return Err(kBadIP);
  addr.sin_addr.s_addr = s_addr;
  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    return Err(kBindError);
  if (listen(sock, SOMAXCONN) == -1) return Err(kListenError);
  if (fcntl(sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
    return Err(kFcntlError);
  Logger::Info() << ip << ":" << port << " : リッスン開始" << std::endl;
  return Ok(sock);
}

uint32_t Server::InetAddr(const std::string& ip_str){
    std::vector<uint8_t> ip_bytes;
    std::stringstream ss(ip_str);
    std::string token;
    while (std::getline(ss, token, '.')) {
      Result<int, std::string> result = string_utils::StrToI(token);
      if (result.IsErr())
        return 0;
      ip_bytes.push_back(result.Unwrap());
    }
    if (ip_bytes.size() != 4) {
        return 0;
    }

    uint32_t ip_addr = 0;
    for (int i = 0; i < 4; ++i) {
        ip_addr += ip_bytes[3 - i] << (8 * (3 - i));
    }
    return ip_addr;
}
