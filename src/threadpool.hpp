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
    
private:
    TaskQueue<std::function<void()>> _tasks;
};

class ThreadWorker {
public:
    ThreadWorker(const ThreadPool& p) : pool(p) {}
private:
    const ThreadPool& pool;
};

}
