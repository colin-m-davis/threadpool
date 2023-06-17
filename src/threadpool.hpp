#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>
#include <cstddef>
#include <thread>
#include <vector>
#include "taskqueue.hpp"

namespace dreadpools {

class ThreadPool {
public:
    ThreadPool(const std::size_t count_threads) {
        threads.resize(count_threads, std::thread(ThreadWorker(*this)));
    }
    bool is_active{true};

private:
    std::vector<std::thread> threads;
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
