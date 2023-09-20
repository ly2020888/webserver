
#include "config_factory.h"
#include "logger.h"
#include "server.h"
int main() {
  auto conf = ConfigFactroy::get_default_server_config();
  Server webapp = Server(std::move(conf));
  webapp.init();
  webapp.start();
  return 0;
}