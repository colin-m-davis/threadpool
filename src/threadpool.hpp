#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>
#include "taskqueue.hpp"

namespace dreadpools {

class ThreadPool {
public:
    bool is_active{true};

private:
    std::condition_variable _cv{};
    std::mutex _cv_m{};
    TaskQueue<std::function<void()>> _tasks{};
    friend class ThreadWorker;
};

class ThreadWorker {
public:
    ThreadWorker(ThreadPool& p) : pool(p) {}
    void operator()();

private:
    ThreadPool& pool;
};

}
