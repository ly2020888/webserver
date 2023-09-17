#include "logger.h"
#include "gtest/gtest.h"

void log() {
  auto &logger = Logger::get_instance();
  int str = 1;
  logger.init();
  logger.set_log_level(DEBUG);

  logger.infof("The logger is initialized {}", str);
  logger.warningf("Congratulations! No Warnings");
  logger.debugf("Congratulations! No Bugs");
  logger.errorf("Congratulations! No Errors");
}
TEST(Logger, testLogger) { log(); }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}