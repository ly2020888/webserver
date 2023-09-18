#include "server.h"
#include "config.h"
#include <cassert>
#include <cstring>
#include <netinet/in.h>
#include <ranges>
#include <sys/epoll.h>
#include <sys/socket.h>

using std::ranges::views::iota;

void Server::set_socket() {

  // 获取套接字
  _listenfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(_listenfd >= 0);
  struct linger tmp = {1, 1};
  setsockopt(_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));

  // 描述IPv4地址
  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(server_config->port);

  // 设置套接字可复用
  int so_reuse_addr_flag = 1;
  setsockopt(_listenfd, SOL_SOCKET, SO_REUSEADDR, &so_reuse_addr_flag,
             sizeof(so_reuse_addr_flag));

  // 套接字绑定到本地地址
  int no_err = bind(_listenfd, (struct sockaddr *)&address, sizeof(address));
  assert(no_err >= 0);

  // 开启监听
  no_err = listen(_listenfd, server_config->connection_queue_max_size);
  assert(no_err >= 0);
}

// 设置epoll对象，size指明epoll对象可以同时处理的文件描述符
// 通常这个值不是固定的，而是可以增长的
void Server::set_epoll() {
  _epollfd = epoll_create(5);
  assert(_epollfd != -1);

  // 首先将服务器的fd注册到epoll对象中
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = _listenfd;
  epoll_ctl(_epollfd, EPOLL_CTL_ADD, _listenfd, &event);
}

void Server::main_loop() {
  while (true) {
    int number = epoll_wait(_epollfd, events.data(), MAX_EVENT_NUMBER, -1);

    for (auto i : iota(0, number)) {
      int client_so_fd = events[i].data.fd;
      // to-do
    }
  }
}

bool Server::register_client() {
  struct sockaddr_in client_address;
  socklen_t client_addrlength = sizeof(client_address);

  if (server_config->listent_trig_mode) { // Edge-Triggered

  } else { // Level-Triggered
    int connfd = accept(_listenfd, (struct sockaddr *)&client_address,
                        &client_addrlength);
    if (connfd < 0) {
      logger.errorf("%s:errno is:%d", "accept error", errno);
      return false;
    }

    logger.infof("The new tcp link has been connected, File descriptor is {:x}",
                 connfd);
    Client c = Client();
    c.initialize(connfd, client_address);
    client_set.register_clinet(std::move(c));
  }
}