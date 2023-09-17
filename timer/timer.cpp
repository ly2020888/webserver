#include "timer.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>

using std::strftime;
using std::string;
using std::chrono::system_clock;

string Timer::format_time() {
  auto current_time = system_clock::now();
  auto time_stamp = system_clock::to_time_t(current_time);
  char timeStr[100];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S",
           std::localtime(&time_stamp));
  return static_cast<string>(timeStr);
}
