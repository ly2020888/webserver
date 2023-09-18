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

class LoggerConfig {
public:
  string file_path;   // 路径名
  string logger_name; // log文件名
  int max_lines;      // 日志最大行数
  LEVEL log_level = INFO;
  void set_level(LEVEL log_level) { this->log_level = log_level; }
  LoggerConfig() = delete;

  explicit LoggerConfig(string file_path = ".",
                        string logger_name = "server.log",
                        int max_lines = 1e5) {
    this->file_path = file_path;
    this->logger_name = logger_name;
    this->max_lines = max_lines;
  }
};

class ServerConfig {

public:
  int conn_trig_mode; // connfd触发模式

  int listent_trig_mode; // listenfd触发模式
                         //
  int mode;              // 触发组合模式
                         //
  int port;              // 端口号

  int sql_thread_num; // 数据库连接数量

  int worker_thread_num; // 线程池内的线程数量

  int connection_queue_max_size; // socket上等待连接队列的最大长度

  ServerConfig() = delete;

  explicit ServerConfig(int mode = 0, int sql_thread_num = 4,
                        int worker_thread_num = 8,
                        int connection_queue_max_size = 10, int port = 6060) {
    this->mode = mode;
    this->listent_trig_mode = (mode & 0x1);
    this->conn_trig_mode = ((mode >> 1) & 0x1);
    this->sql_thread_num = sql_thread_num;
    this->sql_thread_num = sql_thread_num;
    this->worker_thread_num = worker_thread_num;
    this->connection_queue_max_size = connection_queue_max_size;
    this->port = port;
  }
};
