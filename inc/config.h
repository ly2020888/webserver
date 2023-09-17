#pragma once
#include <memory>
#include <string>

using std::make_shared;
using std::shared_ptr;
using std::string;

enum LEVEL {
  ERROR = 0,
  WARNING,
  INFO,
  DEBUG,
};

class Config {
public:
  LEVEL log_level = INFO;
  void set_level(LEVEL log_level) { this->log_level = log_level; }
};

class LoggerConfig : public Config {
public:
  string file_path;   // 路径名
  string logger_name; // log文件名
  int max_lines;      // 日志最大行数
  LoggerConfig() = delete;

  explicit LoggerConfig(string file_path = ".",
                        string logger_name = "server.log",
                        int max_lines = 1e5) {
    this->file_path = file_path;
    this->logger_name = logger_name;
    this->max_lines = max_lines;
  }
};

class ServerConfig : public Config {

public:
  int port = 6060; // 端口号

  int log_write_type; // 日志写入方式

  int mode; // 触发组合模式

  int listent_trig_mode; // listenfd触发模式

  int conn_trig_mode; // connfd触发模式

  int sql_thread_num = 4; // 数据库连接数量

  int worker_thread_num = 8; // 线程池内的线程数量
};
