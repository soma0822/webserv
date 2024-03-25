#include "read_from_cgi.hpp"

ReadFromCGI::ReadFromCGI(int pid, int fd, RequestContext req_ctx,
                         const IConfig &config)
    : AIOTask(fd, POLLIN),
      req_ctx_(req_ctx),
      config_(config),
      parser_(CGIParser()),
      pid_(pid) {}

ReadFromCGI::~ReadFromCGI() {}

// TODO: ここのエラーはクライアントにInternal Server Errorを返す
Result<int, std::string> ReadFromCGI::Execute() {
  (void)config_;
  char buf[buf_size_ + 1];
  int status;
  int len = read(fd_, buf, buf_size_);
  if (len == -1) {
    Logger::Error() << "read エラー" << std::endl;
    return Ok(kFdDelete);
  }
  buf[len] = '\0';
  buf_.append(buf);
  Logger::Info() << "buf_: " << buf_ << std::endl;
  int result = waitpid(pid_, &status, WNOHANG);
  if (result == -1) {  // エラー
    Logger::Error() << "waitpid エラー: " << pid_ << std::endl;
    return Ok(kFdDelete);
  } else if (result == 0) {  // まだ終了していない
    return Ok(kOk);
  }
  if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {  // 異常終了
    Logger::Error() << "cgi エラー" << std::endl;
    IOTaskManager::AddTask(new WriteResponseToClient(
        req_ctx_.fd, GenerateErrorResponse(http::kInternalServerError, config_),
        req_ctx_.request));
    return Ok(kFdDelete);
  }
  if (len == 0) {
    Result<HTTPRequest *, int> result = parser_.Parser(buf_);
    if (result.IsErr()) {
      return Ok(kContinue);
    }
    Option<HTTPResponse *> option =
        CGIHandler::Handle(config_, result.Unwrap(), req_ctx_);
    if (option.IsSome()) {
      HTTPResponse *response = option.Unwrap();
      delete result.Unwrap();
      IOTaskManager::AddTask(
          new WriteResponseToClient(req_ctx_.fd, response, req_ctx_.request));
    }
    return Ok(kFdDelete);
  }
  return Ok(kContinue);
}
