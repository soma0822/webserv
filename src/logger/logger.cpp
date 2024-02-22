#include "logger.hpp"
#include <sstream>

/*
 * Loggerクラス
 */

ILoggerHandler *Logger::handler_ = NULL;

std::ostream &Logger::Info() {
  return Logger::GetInstance().handler_->GetStream() << "[INFO] ";
}

std::ostream &Logger::Warn() {
  return Logger::GetInstance().handler_->GetStream() << "[WARN] ";
}

std::ostream &Logger::Error() {
  return Logger::GetInstance().handler_->GetStream() << "[ERROR] ";
}

std::ostream &Logger::Debug() {
  return Logger::GetInstance().handler_->GetStream() << "[DEBUG] ";
}

void Logger::SetHandler(ILoggerHandler *handler) {
  Logger::GetInstance(); // handler_が初期化される
  delete Logger::handler_;
  Logger::handler_ = handler;
}

Logger::Logger() { Logger::handler_ = new StdoutStreamWrapper(); }

Logger::~Logger() { delete Logger::handler_; }

// GetInstance()の初回呼び出しではコンストラクタが呼ばれるため、handler_が初期化される
Logger &Logger::GetInstance() {
  static Logger instance;
  return instance;
}

/*
 * NullBufferクラス
 */

int NullBuffer::overflow(int c) { return c; }

/*
 * ILoggerHandlerクラス
 */

ILoggerHandler::~ILoggerHandler() {}

/*
 * 標準出力ストリームラッパー
 */

StdoutStreamWrapper::StdoutStreamWrapper() {}

StdoutStreamWrapper::~StdoutStreamWrapper() {}

std::ostream &StdoutStreamWrapper::GetStream() { return std::cout; }

/*
 * ファイルストリームラッパー
 */

FileStreamWrapper::FileStreamWrapper(const std::string &filename)
    : ostream_(filename) {}

FileStreamWrapper::~FileStreamWrapper() { ostream_.close(); }

std::ostream &FileStreamWrapper::GetStream() { return ostream_; }