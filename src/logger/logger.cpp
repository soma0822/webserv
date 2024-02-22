#include "logger.hpp"
#include <sstream>

/*
 * Loggerクラス
 */

std::ostream &Logger::Info() {
  if (Logger::log_level_ > kInfo) {
    return null_stream::null_stream;
  }
  return Logger::GetInstance().handler_->GetStream() << "[INFO] ";
}

std::ostream &Logger::Warn() {
  if (Logger::log_level_ > kWarn) {
    return null_stream::null_stream;
  }
  return Logger::GetInstance().handler_->GetStream() << "[WARN] ";
}

std::ostream &Logger::Error() {
  if (Logger::log_level_ > kError) {
    return null_stream::null_stream;
  }
  return Logger::GetInstance().handler_->GetStream() << "[ERROR] ";
}

std::ostream &Logger::Debug() {
  if (Logger::log_level_ > kDebug) {
    return null_stream::null_stream;
  }
  return Logger::GetInstance().handler_->GetStream() << "[DEBUG] ";
}

void Logger::SetHandler(ILoggerHandler *handler) {
  Logger::GetInstance(); // handler_が初期化される
  delete Logger::handler_;
  Logger::handler_ = handler;
}

void Logger::SetLogLevel(LogLevel level) { Logger::log_level_ = level; }

ILoggerHandler *Logger::handler_ = NULL;
LogLevel Logger::log_level_ = kInfo;

// コンストラクタ, デストラクタ, コピーコンストラクタ,
// コピー代入演算子はprivateにする
Logger::Logger() { Logger::handler_ = new StdoutStreamWrapper(); }

Logger::Logger(const Logger &other) {}

Logger &Logger::operator=(const Logger &other) { return *this; }

Logger::~Logger() { delete Logger::handler_; }

// GetInstance()の初回呼び出しではコンストラクタが呼ばれるため、handler_が初期化される
Logger &Logger::GetInstance() {
  static Logger instance;
  return instance;
}

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

std::ostream &FileStreamWrapper::GetStream() {
  if (!ostream_.is_open()) {
    return null_stream::null_stream;
  }
  return ostream_;
}