#include "config_factory.h"
#include "http.h"
#include "logger.h"
#include "server.h"
#include "gtest/gtest.h"
#include <memory>
#include <vector>

TEST(logger, testLogger) {
  auto &logger = Logger::get_instance();
  int str = 1;
  logger.init();
  logger.set_log_level(DEBUG);
  logger.infof("The logger is initialized {}", str);
  logger.warningf("Congratulations! No Warnings");
  logger.debugf("Congratulations! No Bugs");
  logger.errorf("Congratulations! No Errors");
}

TEST(ServerSocket, testServer) {
  auto conf = ConfigFactroy::get_default_server_config();
  Server webapp = Server(std::move(conf));
  webapp.init();
}
