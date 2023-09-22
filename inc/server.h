#pragma once
#include "clients.h"
#include "config.h"
#include "logger.h"
#include <array>
#include <sys/epoll.h>
#include <sys/socket.h>

using std::array;
using ServerConfigPtr = shared_ptr<ServerConfig>;
const int MAX_EVENT_NUMBER = 1e4;

class Server {

public:
  void main_loop();
  void init();
  void start();
  Server(ServerConfigPtr conf) : server_config(conf){};
  Server(const Server &) = delete;
  Server(Server &&server) = delete;
  Server &operator=(Server &&server) = delete;
  Server &operator=(const Server &) = delete;
  ~Server() {
    // 关闭所有的连接
    client_set.close_all();

    // 关闭所有线程
  }

private:
  Logger &logger = Logger::get_instance();
  Clients client_set;
  ServerConfigPtr server_config;
  array<struct epoll_event, MAX_EVENT_NUMBER> events;

  int _epollfd;  // epoll对象
  int _listenfd; // 监听套接字

  void set_socket();
  void set_epoll();
  bool register_client();
};
