#include "logger.h"
#include "config.h"
#include "config_factory.h"
#include "timer.h"
#include <fmt/core.h>
#include <memory>
#include <sstream>
#include <string_view>

#define CRESET "\e[0m"
#define CRED "\e[31m"    /* Red */
#define CGREEN "\e[32m"  /* Green */
#define CYELLOW "\e[33m" /* Yellow */
#define CBLUE "\e[34m"   /* Blue */
#define CWHITE "\e[37m"  /* White */

void Logger::init() {

  ConfigFactroy factory = ConfigFactroy();
  this->config = factory.get_default_logger_config();
  this->_level = this->config->log_level;
  this->lw = std::make_shared<LogWriter>("server.log", 1, *this->config);
  lw->background_deamon(); // start deamon thread
}

void Logger::write_log(std::string_view sw) { lw->write_log(sw); }

void Logger::set_log_level(LEVEL level) { _level = level; }

void Logger::set_color(string &origin, Color colors) {
  std::stringstream ss;
  switch (colors) {
  case RED:
    ss << CRED << origin << CRESET;
    break;

  case BLUE:
    ss << CBLUE << origin << CRESET;
    break;
  case YELLOW:
    ss << CYELLOW << origin << CRESET;
    break;
  case GREEN:
    ss << CGREEN << origin << CRESET;
    break;
  default:
    ss << CWHITE << origin << CRESET;
  }
  origin = ss.str();
}