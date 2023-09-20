#include "config_factory.h"
#include "logger.h"
#include "server.h"
#include "gtest/gtest.h"
#include <memory>

void log() {
  auto &logger = Logger::get_instance();
  int str = 1;
  logger.init();
  logger.set_log_level(DEBUG);

  logger.infof("The logger is initialized {}", str);
  logger.warningf("Congratulations! No Warnings");
  logger.debugf("Congratulations! No Bugs");
  logger.errorf("Congratulations! No Errors");
  // ASSERT_EQ(1, 2) << "Vectors 1 and 1 are of unequal length";
}
// TEST(Logger, testLogger) { log(); }

TEST(ServerSocket, testServer) {
  auto conf = ConfigFactroy::get_default_server_config();
  Server webapp = Server(std::move(conf));
  webapp.init();
  webapp.start();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}