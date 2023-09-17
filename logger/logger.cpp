#include "logger.h"
#include "config.h"
#include "config_factory.h"
#include "timer.h"
#include <fmt/core.h>
#include <memory>
#include <string_view>

void Logger::init() {

  ConfigFactroy factory = ConfigFactroy();
  this->config = factory.get_default_logger_config();
  this->_level = this->config->log_level;
  this->lw = std::make_shared<LogWriter>("server.log", 1, *this->config);
  lw->background_deamon(); // start deamon thread
}

void Logger::write_log(std::string_view sw) { lw->write_log(sw); }

void Logger::set_log_level(LEVEL level) { _level = level; }