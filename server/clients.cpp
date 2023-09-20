#include "clients.h"
#include "errors.h"
#include <fcntl.h>
#include <fmt/core.h>
#include <sys/epoll.h>
#include <unistd.h>

// 客户端
int Client::get_connfd() { return connfd; }

struct sockaddr_in Client::get_socket() { return client_address; }

void Client::client_epoll_ctl() {
  epoll_event event;
  event.data.fd = connfd;

  // if (trigger_mode)
  event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
  // else
  // event.events = EPOLLIN | EPOLLRDHUP;

  epoll_ctl(_epollfd, EPOLL_CTL_ADD, connfd, &event);

  int old_option = fcntl(connfd, F_GETFL);
  int new_option = old_option | O_NONBLOCK;
  fcntl(connfd, F_SETFL, new_option);
}

void Client::read_once() {
  size_t bytes_read = 0;
  while (true) {
    bytes_read =
        recv(connfd, read_buffer.get_raw(), read_buffer.unused_size(), 0);
    read_buffer.write(bytes_read);

    if (bytes_read == -1) {
      // 非阻塞ET模式下，需要一次性将数据读完，无错
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      } else {
        string msg = fmt::format("The client {:x} failed to read data", connfd);
        throw ReadError(msg);
      }
    }

    if (bytes_read == 0) {
      string msg =
          fmt::format("client {:x}: there is no readable data", connfd);
      throw ReadError(msg);
    }
  }
}
void Client::disconnect() { _collection.remove_client(connfd); }

void Client::write_once() {
  // ssize_t send(int sockfd, const void *buf, size_t len, int flags);
  size_t bytes_write = 0;
  while (!write_buffer.empty()) {
    bytes_write = send(connfd, write_buffer.get_raw(), write_buffer.size(), 0);

    if (bytes_write < 0) {
      if (errno == EAGAIN) {
        pay_attention_to(EPOLLOUT);

      } else {
        disconnect();
        //  抛出写错误
      }
      return;
    }
    write_buffer.read(bytes_write);
  }
}

// 关注读事件，但是oneshot模式，即当读时间通知后，下次关注
// 读时间则需再次打开。
void Client::pay_attention_to(int epollout) {
  epoll_event event;
  event.data.fd = connfd;
  event.events = epollout | EPOLLET | EPOLLONESHOT | EPOLLRDHUP | EPOLLONESHOT;
  epoll_ctl(_epollfd, EPOLL_CTL_MOD, connfd, &event);
}
// 客户端集合
void Clients::remove_client(int connfd) {
  connfds.erase(connfd);
  collection.erase(connfd);
}

void Clients::register_client(ClientPtr cp) {
  int connfd = cp->get_connfd();
  cp->client_epoll_ctl();
  collection[connfd] = std::move(cp);
  connfds.insert(connfd);
}

void Clients::read_from(int connfd) {
  try {
    collection[connfd]->read_once();
  } catch (const std::exception &e) {
    throw e;
  }
}

void Clients::write_from(int connfd) {
  try {
    collection[connfd]->write_once();
  } catch (const std::exception &e) {
    throw e;
  }
}
void Clients::close_all() {
  for (auto &client : connfds) {
    close(client);
  }
  connfds.clear();
  collection.clear();
}
