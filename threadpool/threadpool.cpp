#include "threadpool.h"
#include <ranges>
using std::ranges::views::iota;

void work(TaskQueuePtr taskQueue) {
  while (true) {
    Task task = taskQueue->pop();
    if (task.exec()) {
      return;
    }
  }
}

void ThreadPool::start() {
  for (auto i : iota(0, config->min_thread_number)) {
    threads.emplace_back(work, std::ref(this->tq));
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

void ThreadPool::add_job(Task tsk) { tq->push(std::move(tsk)); }

// 添加线程池守护进程，动态规划线程数量