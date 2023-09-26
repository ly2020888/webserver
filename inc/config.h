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
  LEVEL log_level = DEBUG;
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

  int mode; // 触发组合模式

  int port; // 端口号

  int sql_thread_num; // 数据库连接数量

  int worker_thread_num; // 线程池内的线程数量

  int connection_queue_max_size; // socket上等待连接队列的最大长度

  ServerConfig(int mode = 0, int sql_thread_num = 4, int worker_thread_num = 8,
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

class ThreadPoolConfig {
public:
  int max_thread_number; // 线程池中的线程数

  int min_thread_number; // 线程池中的最小线程数

  int reclamation_rate; // 回收工作线程的频率，以分钟计时

  int creation_rate; // 创建线程的工作时间，以分钟计时

  int quantity; // 新增线程的数量，将在两次时间间隔内均匀创建

  int m_max_requests; // 请求队列中允许的最大请求数

  int time_out; // 任务超时时间
  ThreadPoolConfig(int max_thread_number = 16, int min_thread_number = 4,
                   int reclamation_rate = 1, int creation_rate = 1,
                   int quantity = 4, int m_max_requests = 1024,
                   int time_out = 5);
};