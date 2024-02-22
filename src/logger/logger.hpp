#ifndef WEBSERV_SRC_LOGGER_LOGGER_HPP
#define WEBSERV_SRC_LOGGER_LOGGER_HPP

#include <fstream>
#include <iostream>

class ILoggerHandler;
class NullBuffer;

enum LogLevel {
  kInfo = 0,
  kWarn = 1,
  kError = 2,
  kDebug = 3,
};

/*
 * Loggerクラス
 *
 * SetHandler()でログ出力先を設定し、Info(), Warn(), Error()でログを出力する
 * SetHandler()を呼び出さない場合は標準出力にログを出力する
 *
 * Usage:
 *  if (log_to_file) {
 *    Logger::SetHandler(new FileStreamWrapper("log.txt"));
 *  }
 *  Logger::Info() << "Info message" << std::endl;
 *  Logger::Warn() << "Warn message" << std::endl;
 *  Logger::Error() << "Error message" << std::endl;
 */

class Logger {
public:
  static std::ostream &Info();
  static std::ostream &Warn();
  static std::ostream &Error();
  static std::ostream &Debug();

  static void SetHandler(ILoggerHandler *handler);
  static void SetLogLevel(LogLevel level);

private:
  static ILoggerHandler *handler_;
  static NullBuffer null_buffer_;
  static std::ostream null_stream_;
  static LogLevel log_level_;

  // 直接インスタンス化できないようにする
  Logger();
  ~Logger();

  static Logger &GetInstance();
};

class NullBuffer : public std::streambuf {
public:
  int overflow(int c);
};

/*
 * ostream wrapper
 */

class ILoggerHandler {
public:
  virtual ~ILoggerHandler();
  virtual std::ostream &GetStream() = 0;
};

/*
 * 標準出力ストリームラッパー
 */

class StdoutStreamWrapper : public ILoggerHandler {
public:
  StdoutStreamWrapper();
  ~StdoutStreamWrapper();

  std::ostream &GetStream();
};

/*
 * ファイルストリームラッパー
 */

class FileStreamWrapper : public ILoggerHandler {
public:
  FileStreamWrapper(const std::string &filename);
  ~FileStreamWrapper();

  std::ostream &GetStream();

private:
  std::ofstream ostream_;
};

#endif // WEBSERV_SRC_LOGGER_LOGGER_HPP
