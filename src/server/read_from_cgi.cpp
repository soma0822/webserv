#include "read_from_cgi.hpp"

ReadFromCGI::ReadFromCGI(int pid, int fd, RequestContext req_ctx,
                         const IConfig &config, timespec ts)
    : AIOTask(fd, POLLIN | POLLHUP, kCGIFd),
      req_ctx_(req_ctx),
      config_(config),
      parser_(CGIParser()),
      pid_(pid),
      ts_(ts) {}

ReadFromCGI::~ReadFromCGI() {}

Result<int, std::string> ReadFromCGI::Execute(int revent) {
  int status;
  int result = waitpid(pid_, &status, WNOHANG);
  if (result == -1) {
    Logger::Error() << "waitpid エラー: " << pid_ << std::endl;
    IOTaskManager::AddTask(new WriteResponseToClient(
        req_ctx_.fd, GenerateErrorResponse(http::kInternalServerError, config_),
        req_ctx_.request));
    return Ok(kFdDelete);
  } else if (result != 0 && (WIFEXITED(status) == 0 ||
                             WEXITSTATUS(status) != 0)) {  // 異常終了の判定
    Logger::Error() << "cgi エラー" << std::endl;
    IOTaskManager::AddTask(new WriteResponseToClient(
        req_ctx_.fd, GenerateErrorResponse(http::kInternalServerError, config_),
        req_ctx_.request));
    return Ok(kFdDelete);
  }
  if (!sended_signal_ && result == 0 &&
      time_utils::TimeOut(ts_, child_process_timeout_)) {  // タイムアウトの判定
    Logger::Error() << "CGIがタイムアウトしました" << std::endl;
    kill(
        pid_,
        SIGKILL);  // シグナルで子プロセスを終了させて、次のexecuteで異常終了の判定をする
    sended_signal_ = true;
    return Ok(kContinue);
  }
  if (!(event_ & revent)) return Ok(kNotReady);  // fdのreadyの判定
  (void)config_;
  char buf[buf_size_ + 1];
  int len = read(
      fd_, buf, buf_size_);  // 子プロセスの正常終了、実行中にかかわらず読み込む
  if (len == -1) {
    Logger::Error() << "read エラー" << std::endl;
    IOTaskManager::AddTask(new WriteResponseToClient(
        req_ctx_.fd, GenerateErrorResponse(http::kInternalServerError, config_),
        req_ctx_.request));
    return Ok(kFdDelete);
  }
  buf[len] = '\0';
  buf_.append(buf);
  if (result == 0) {  // 子プロセス実行中なら読み終わってないので待つ
    return Ok(kContinue);
  }
  if (len == 0) {  // eofを読んだのでレスポンスを返す
    Result<HTTPRequest *, int> result = parser_.Parser(buf_);
    if (result.IsErr()) {
      return Ok(kContinue);
    }
    Logger::Info() << "CGIからリクエストを受け取りました\n\n"
                   << *result.Unwrap() << std::endl;
    Option<HTTPResponse *> option =
        CGIHandler::Handle(config_, result.Unwrap(), req_ctx_);
    if (option.IsSome()) {
      HTTPResponse *response = option.Unwrap();
      delete result.Unwrap();
      IOTaskManager::AddTask(
          new WriteResponseToClient(req_ctx_.fd, response, req_ctx_.request));
    }
    return Ok(kFdDelete);
  } else  // eofを読むまで待つ
    return Ok(kContinue);
}
