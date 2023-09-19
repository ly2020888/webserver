#include "clients.h"
#include "config.h"
#include "logger.h"
#include <array>
#include <sys/epoll.h>
#include <sys/socket.h>

using std::array;
const int MAX_EVENT_NUMBER = 1e4;

class Server {

public:
  void main_loop();

private:
  Logger &logger = Logger::get_instance();
  Clients client_set;
  shared_ptr<ServerConfig> server_config;
  array<struct epoll_event, MAX_EVENT_NUMBER> events;

  int _epollfd;  // epoll对象
  int _listenfd; // 监听套接字

  void set_socket();
  void set_epoll();
  bool register_client();
};
