#pragma once
#include "config.h"
#include "task.h"
#include <memory>
using std::shared_ptr;
using TaskQueuePtr = shared_ptr<TaskQueue>;

class ThreadPool {
public:
  void start();
  void add_job(Task tsk);
  static shared_ptr<ThreadPool> create(shared_ptr<ThreadPoolConfig> config) {
    auto tp = make_shared<ThreadPool>(config);
    return tp;
  };

private:
  shared_ptr<ThreadPoolConfig> config;
  std::vector<std::thread> threads;
  TaskQueuePtr tq;
  int number;
  ThreadPool(shared_ptr<ThreadPoolConfig> config) : config(config){};
};