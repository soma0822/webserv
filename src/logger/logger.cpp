#include "logger.hpp"
#include <fstream>

/*
 * Loggerクラス
 */

ILoggerHandler *Logger::handler_ = NULL;

std::ostream &Logger::Info() {
  return Logger::GetInstance().GetHandler()->GetStream() << "[INFO] ";
}

std::ostream &Logger::Warn() {
  return Logger::GetInstance().GetHandler()->GetStream() << "[WARN] ";
}

std::ostream &Logger::Error() {
  return Logger::GetInstance().GetHandler()->GetStream() << "[ERROR] ";
}

std::ostream &Logger::Debug() {
  return Logger::GetInstance().GetHandler()->GetStream() << "[DEBUG] ";
}

void Logger::SetHandler(ILoggerHandler *handler) {
  ILoggerHandler *old_handler = Logger::GetInstance().GetHandler();
  if (old_handler) {
    delete old_handler;
  }
  Logger::handler_ = handler;
}

ILoggerHandler *Logger::GetHandler() { return Logger::handler_; }

Logger::Logger() { Logger::handler_ = new StdoutStreamWrapper(); }

Logger::~Logger() {}

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
  std::cout << "FileStreamWrapper::GetStream" << std::endl;
  return ostream_;
}