
#include "logger.h"

int main() {
  auto &logger = Logger::get_instance();
  int str = 1;
  logger.init();
  logger.set_log_level(DEBUG);
  // logger.infof("The logger is initialized {}", str);
  // logger.warningf("Congratulations! No Warnings");
  logger.debugf("Congratulations! No Bugs");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  logger.debugf("Congratulations! No Bugs");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  logger.debugf("Congratulations! No Bugs");

  return 0;
}