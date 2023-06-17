#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>
#include <cstddef>
#include <thread>
#include <vector>
#include <iostream>
#include <stop_token>
#include "taskqueue.hpp"

namespace dreadpools {

class ThreadPool {
public:
    ThreadPool(const std::size_t count_threads);
    bool is_active{false};

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
    // std::stop_token stop_token;
    ThreadPool& pool;
};

}
