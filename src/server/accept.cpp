#include "accept.hpp"

Accept::Accept(const std::string &port){
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  Result<int, std::string> result = string_utils::StrToI(port);

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(result.Unwrap());
  addr.sin_addr.s_addr = INADDR_ANY;

// バインドできない、リッスンできないはコンフィグで指定されたportに問題があると思われるのでthrowで抜ける。
  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    throw std::invalid_argument(port + " : bindエラー");
  if (listen(sock, SOMAXCONN) == -1)
    throw std::invalid_argument(port + " : listenエラー"); 
  event_ = POLL_IN;
  port_ = port;
  fd_ = sock;
}

Accept::~Accept() {}

Result<int, std::string> Accept::Execute(){
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(client_addr);
  int client_sock = accept(fd_, (struct sockaddr *)&client_addr, &len);
  if (client_sock == -1){
    Logger::Error() << "accept エラー" << std::endl;
    return Err("accept error");
  }
  Logger::Info() << port_ << " : 接続しました" << std::endl;
  //TODO: IOTaskManagerクラスとReadRequestFromClientクラスの実装
  // IOTaskManager::AddTask(new ReadRequestFromClient(client_sock, port_));
}