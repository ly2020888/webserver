#include "buffer.h"
#include <memory>
#include <netinet/in.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::unordered_map;
using std::unordered_set;
using std::vector;
constexpr int KB = 1024;
class Clients;
class Client;
using ClientPtr = std::unique_ptr<Client>;

class Client {
public:
  // static void set_trigger_mode(int mode) { trigger_mode = mode; }

  int get_connfd();
  void client_epoll_ctl();
  void read_once();
  void write_once();
  void pay_attention_to(int epollout);
  void disconnect();
  struct sockaddr_in get_socket();

  void initialize(int connfd, struct sockaddr_in client_address) {
    this->connfd = connfd;
    this->client_address = client_address;
  }

  static ClientPtr Create(Clients &belongs, int epollfd,
                          int buffer_size = 2 * KB) {
    return std::make_unique<Client>(belongs, buffer_size);
  }

  Client(Clients &belongs, int epollfd, int buffer_size = 2 * KB)
      : read_buffer(buffer_size), write_buffer(buffer_size),
        _collection(belongs), _epollfd(epollfd) {}

private:
  // static int trigger_mode;
  int _epollfd;
  int connfd;
  Buffer read_buffer;
  Buffer write_buffer;
  Clients &_collection;
  struct sockaddr_in client_address;
};

class Clients {
public:
  void register_client(ClientPtr cp);
  void remove_client(int connfd);
  void read_from(int connfd);
  void write_from(int connfd);
  void close_all();

private:
  unordered_map<int, ClientPtr> collection;
  unordered_set<int> connfds;
};
