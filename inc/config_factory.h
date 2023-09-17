#pragma once
#include "config.h"
#include <memory>
#include <string>

using std::make_shared;
using std::shared_ptr;
using std::string;

class ConfigFactroy {
public:
  shared_ptr<LoggerConfig> get_default_logger_config() {
    auto ptr = make_shared<LoggerConfig>(".", "server.log", 100);
    return ptr;
  }
  // shared_ptr<ServerConfig> get_default_server_config() {
  //   auto ptr = make_shared<ServerConfig>(new ServerConfig());
  //   return ptr;
  // }
};