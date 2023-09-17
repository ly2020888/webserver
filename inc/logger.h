#pragma once

#include "config.h"
#include "timer.h"
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

using std::fstream;
using std::lock_guard;
using std::make_shared;
using std::mutex;
using std::queue;
using std::shared_ptr;
using std::string;
using std::thread;
using std::unique_ptr;

// logger 支持 日志等级
// 日志可以选择 warning  info debug error 四种等级
// 日志支持文件输出
// 日志支持文件配置 *
// 日志支持自定义的错误输出
// 日志支持打印到屏幕上
class Logger;
class LogWriter {

  string file_name;
  fstream *file;
  mutex mute;
  queue<std::string_view> q;
  unique_ptr<LoggerConfig> config;
  int log_flush_time;

public:
  LogWriter() = default;
  LogWriter(string filename, int seconds, LoggerConfig &conf)
      : file_name(filename), log_flush_time(seconds) {
    file = new fstream();
    file->open(filename, std::ios::app);
    config = std::make_unique<LoggerConfig>(conf);
  }

  ~LogWriter() {
    file->close();
    delete file;
    file = nullptr;
  }

  void write_log(std::string_view str) {
    lock_guard<mutex> lock(mute);
    if (q.size() < config->max_lines) {
      q.push(str);
    }
  }

  void flush() {
    lock_guard<mutex> lock(mute);

    while (q.size() > 0) {
      std::string_view str = q.front();
      q.pop();
      try {
        (*file) << str << std::endl;
      } catch (const std::exception &e) {
        std::cerr << "flush log queue error: " << e.what() << std::endl;
        throw e;
      }
    }
  }

  void background_deamon() {
    auto period = [this]() {
      while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(this->log_flush_time));

        if (q.size() > 0) {
          try {
            this->flush();
          } catch (const std::exception &e) {
            std::cerr << "background_deamon exited: " << e.what() << std::endl;
            return;
          }
        }
      }
    };
    std::thread deamon(period);
    deamon.detach();
  }
};

class Logger {
public:
  static Logger &get_instance() {
    static Logger instance;
    return instance;
  }

public:
  template <typename... Args> void errorf(std::string_view s, Args... args);
  template <typename... Args> void infof(std::string_view s, Args... args);
  template <typename... Args> void warningf(std::string_view s, Args... args);
  template <typename... Args> void debugf(std::string_view s, Args... args);
  void init();
  void write_log(std::string_view);
  void set_log_level(LEVEL level);
  ~Logger() { lw->flush(); }

private:
  Logger(){};
  Logger(Logger const &);
  Logger &operator=(Logger const &);
  LEVEL _level;

  shared_ptr<LoggerConfig> config;
  shared_ptr<LogWriter> lw;
};

template <typename... Args>
void Logger::errorf(std::string_view s, Args... args) {
  if (_level >= ERROR) {
    string input(s);
    auto tmstr = Timer::get_instance().format_time() + " [ERROR] " + input;
    auto str = fmt::format(tmstr, args...);
    std::cout << str << std::endl;
    write_log(str);
  }
}

template <typename... Args>
void Logger::infof(std::string_view s, Args... args) {
  if (_level >= INFO) {
    string input(s);
    auto tmstr = Timer::get_instance().format_time() + " [INFO] " + input;
    auto str = fmt::format(tmstr, args...);
    std::cout << str << std::endl;
    write_log(str);
  }
}

template <typename... Args>
void Logger::warningf(std::string_view s, Args... args) {
  if (_level >= WARNING) {
    string input(s);
    auto tmstr = Timer::get_instance().format_time() + " [WARING] " + input;
    auto str = fmt::format(tmstr, args...);
    std::cout << str << std::endl;
    write_log(str);
  }
}

template <typename... Args>
void Logger::debugf(std::string_view s, Args... args) {
  if (_level >= DEBUG) {
    string input(s);
    auto tmstr = Timer::get_instance().format_time() + " [DEBUG] " + input;
    auto str = fmt::format(tmstr, args...);
    std::cout << str << std::endl;
    write_log(str);
  }
}
