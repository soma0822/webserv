#ifndef WEBSERV_SRC_LOGGER_LOGGER_HPP
#define WEBSERV_SRC_LOGGER_LOGGER_HPP

#include "null_stream.hpp"
#include <fstream>
#include <iostream>

class ILoggerHandler;

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
 *  Logger::SetLogLevel(kWarn);
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
  static null_stream::NullStream null_stream_;
  static ILoggerHandler *handler_;
  static LogLevel log_level_;

  // コンストラクタ, デストラクタ, コピーコンストラクタ,
  // コピー代入演算子はprivateにする
  Logger();
  Logger(const Logger &other);
  Logger &operator=(const Logger &other);
  ~Logger();

  static Logger &GetInstance();
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
  FileStreamWrapper();
  ~FileStreamWrapper();

  std::ostream &GetStream();

private:
  static const std::string kDefaultLogDir;
  std::ofstream ostream_;
  null_stream::NullStream null_stream_;
};

#endif // WEBSERV_SRC_LOGGER_LOGGER_HPP
