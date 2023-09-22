#pragma once
#include "config.h"
#include "task.h"
#include <memory>
using std::shared_ptr;
using TaskQueuePtr = shared_ptr<TaskQueue>;

class ThreadPool {
  void init();
  void add_job(Task tsk);

private:
  shared_ptr<ThreadPoolConfig> config;
  std::vector<std::thread> threads;
  TaskQueuePtr tq;
  int number;
};