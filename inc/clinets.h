#include <netinet/in.h>
#include <unordered_map>
#include <vector>
using std::unordered_map;
using std::vector;

class Client {
public:
  int get_connfd() { return connfd; }
  struct sockaddr_in get_socket() { return client_address; }
  void initialize(int connfd, struct sockaddr_in client_address) {
    this->connfd = connfd;
    this->client_address = client_address;
  }

private:
  int connfd;
  struct sockaddr_in client_address;
};

class Clients {
public:
  void register_clinet(Client cc) {
    int connfd = cc.get_connfd();
    connfds.push_back(connfd);
    collection[connfd] = cc;
  }

private:
  vector<int> connfds;
  unordered_map<int, Client> collection;
};
