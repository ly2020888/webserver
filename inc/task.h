#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
using std::condition_variable;
using std::function;
using std::mutex;
using std::queue;

class Task {
public:
  template <typename Func, typename... Args>
  Task(Func &&func, Args &&...args)
      : exit(false), _func(std::bind(std::forward<Func>(func),
                                     std::forward<Args>(args)...)) {}

  bool exec() {
    _func();
    return exit.load();
  }
  void exit_after() { exit = true; }

private:
  std::atomic<bool> exit;
  function<void()> _func;
};

template <typename Element> class UnboundedChannel {
public:
  void push(Element e) {
    std::lock_guard<mutex> lock(_mutex);
    _tasks.push(e);
  };

  Element pop() {
    std::lock_guard<mutex> lock(_mutex);
    _cv.wait(lock, [this] { return !_tasks.empty(); });
    Element tmp = std::move(_tasks.front());
    _tasks.pop();
    return tmp;
  };

private:
  queue<Element> _tasks;
  mutex _mutex;
  condition_variable _cv;
};

using TaskQueue = UnboundedChannel<Task>;
