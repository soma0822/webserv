#ifndef WEBSERV_SRC_LOGGER_LOGGER_HPP
#define WEBSERV_SRC_LOGGER_LOGGER_HPP

#include <fstream>
#include <iostream>

class ILoggerHandler;

class Logger {
public:
  static std::ostream &Info();
  static std::ostream &Warn();
  static std::ostream &Error();
  static std::ostream &Debug();

  static void SetHandler(ILoggerHandler *handler);
  static ILoggerHandler *GetHandler();

private:
  static ILoggerHandler *handler_;

  // 直接インスタンス化できないようにする
  Logger();
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
  ~FileStreamWrapper();

  std::ostream &GetStream();

private:
  std::ofstream ostream_;
};

#endif // WEBSERV_SRC_LOGGER_LOGGER_HPP
