#include "threadpool.h"
#include <ranges>
using std::ranges::views::iota;
void work_thread(TaskQueuePtr taskQueue) {
  while (true) {
    Task task = taskQueue->pop();
    task.exec();
  }
}

void ThreadPool::init() {
  for (auto i : iota(0, config->min_thread_number)) {
    threads.emplace_back(work_thread, std::ref(this->tq));
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

void ThreadPool::add_job(Task tsk) { tq->push(std::move(tsk)); }