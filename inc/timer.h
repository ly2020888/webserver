#pragma once
#include <string>
using std::string;

class Timer {
private:
  Timer(){};
  Timer(Timer const &);
  Timer &operator=(Timer const &);

public:
  static Timer &get_instance() {
    static Timer instance;
    return instance;
  };

  string format_time();
};